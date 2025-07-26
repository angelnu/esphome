#include <format>
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "SiedleInhomeBus.h"


namespace esphome {
namespace siedle_inhome_bus {

static const char *TAG = "siedle_inhome_bus.component";

static const uint32_t BIT_DURATION_US = 2000;
static const uint8_t TICKS_PER_BIT = 4;

void SiedleInhomeBus::setup() {
    // Inputs
    this->carrier_pin_->setup();
    this->rx_pin_->setup();
    //Outputs
    this->load_pin_->setup();
    this->load_pin_->digital_write(false);
    this->tx_pin_->setup();
    this->tx_pin_->digital_write(true);

    this->isr_carrier_pin_ = this->carrier_pin_->to_isr();
    this->isr_rx_pin_      = this->rx_pin_->to_isr();
    this->isr_load_pin_    = this->load_pin_->to_isr();
    this->isr_tx_pin_      = this->tx_pin_->to_isr();

    this->carrier_pin_->attach_interrupt(&SiedleInhomeBus::s_gpio_intr, this,
                                         gpio::INTERRUPT_RISING_EDGE);
    
    this->bus_timer_ = timerBegin(1000000);
    auto casted_bus_timer = reinterpret_cast<void (*)(void *)>(&SiedleInhomeBus::s_timer_intr);
    timerAttachInterruptArg(this->bus_timer_, casted_bus_timer, this);
    // For ESP32, we can't use dynamic interval calculation because the timerX functions
    // are not callable from ISR (placed in flash storage).
    // Here we just use an interrupt firing every 500 µs.
    timerAlarm(this->bus_timer_, BIT_DURATION_US / TICKS_PER_BIT, true, 0);
}

void SiedleInhomeBus::loop() {

    if (this->last_gpio_interrupt_at_ != 0) {
        ESP_LOGI(TAG, "Interrupt received at %" PRIu32, this->last_gpio_interrupt_at_);
        this->last_gpio_interrupt_at_ = 0;
    }
    
    if (this->last_command_complete_at_ != 0) {
        ESP_LOGI(TAG, "Command completed at %" PRIu32, this->last_command_complete_at_);
        this->last_command_complete_at_ = 0;
    }
    
    if (this->aborted_at_pin_ != 0) {
        ESP_LOGE(TAG, "Aborted receive at bit %d", this->aborted_at_pin_-100);
        this->aborted_at_pin_ = 0;
    }

    // Reset enabled binary sensors (previous loop)
    for(const auto& [command, binary_sensor] : this->binary_sensors_){
        if (binary_sensor->get_state()) {
            binary_sensor->publish_state(false);
        }
    }

    if (is_received_cmd()) {
        auto cmd = get_received_cmd();

        if (enabled_dump_) {
            ESP_LOGI (TAG, "Received CMD: %s", format_hex_pretty(cmd).c_str());
        }

        auto binary_sensor_itr = this->binary_sensors_.find(cmd);
        if (binary_sensor_itr != this->binary_sensors_.end()) {
            // found
            binary_sensor_itr -> second -> publish_state(true);
        }
    }

}

void SiedleInhomeBus::dump_config(){
    ESP_LOGCONFIG(TAG, "Siedle In-Home Bus");

    LOG_PIN("  Carrier Pin: ", this->carrier_pin_);
    LOG_PIN("  RX Pin:      ", this->rx_pin_);
    LOG_PIN("  Load Pin:    ", this->load_pin_);
    LOG_PIN("  TX Pin:      ", this->tx_pin_);

    // TBD - remove
    ESP_LOGI(TAG, "Carrier: %d RX: %d", this->carrier_pin_->digital_read(), this->rx_pin_->digital_read());
    ESP_LOGI(TAG, "Load: %d TX: %d", this->load_pin_->digital_read(), this->tx_pin_->digital_read());

    for(const auto& [command, binary_sensor] : this->binary_sensors_){
        auto desc = std::format("Binary Sensor - Comand ({}):", format_hex_pretty(command));
        LOG_BINARY_SENSOR("  ", desc.c_str(), binary_sensor);
    }
}


/// GPIO interrupt routine, called when incoming msg
void IRAM_ATTR HOT SiedleInhomeBus::gpio_intr() {

    this->last_gpio_interrupt_at_ = micros();

    // Abort if not idle
    if (this->bus_status_ != SiedleInhomeBus::idle) {
        //Cannot read new cmd if not idle
        return;
    }

    // Start read
    this->bus_status_ = SiedleInhomeBus::receiving;

    // Wait for mid of the bit
    this->bit_ticks_left_ = TICKS_PER_BIT / 2;
    this->bits_left_ = 32;

}

void IRAM_ATTR HOT SiedleInhomeBus::s_gpio_intr(SiedleInhomeBus *arg) {
    arg->gpio_intr();
}


void SiedleInhomeBus::send_cmd(uint32_t cmd) {

    ESP_LOGI(TAG, "Sending command: %s at %" PRIu32, format_hex_pretty(cmd).c_str(), micros());
    
    noInterrupts();
    if (this->bus_status_ != SiedleInhomeBus::idle) {
        interrupts();
        //Cannot transfer new cmd if not idle
        ESP_LOGE(TAG, "send_cmd called while not idle: %d", this->bus_status_);
        return;
    }

    //Start sending
    this->transferred_cmd_ = cmd;
    this->bus_status_ = SiedleInhomeBus::sending;
    
    timerRestart(this->bus_timer_);
    this->bit_ticks_left_ = TICKS_PER_BIT;
    this->bits_left_ = 31;

    // Enable load to write
    this->load_pin_->digital_write(true);
    bool bit_to_send = bitRead(this->transferred_cmd_, this->bits_left_);
    this->tx_pin_->digital_write(bit_to_send);

    interrupts();
}

/// Timmer interrupt routine, called each BIT_DURATION_US / TICKS_PER_BIT
void IRAM_ATTR HOT SiedleInhomeBus::timer_intr() {

    if (this->bus_status_ != SiedleInhomeBus::receiving && this->bus_status_ != SiedleInhomeBus::sending) {
        return;
    }

    this->bit_ticks_left_--;
    if ( this->bit_ticks_left_ > 0) {
        return;
    }
    
    switch (this->bus_status_)
    {
    case SiedleInhomeBus::receiving: {
        if (!this->isr_carrier_pin_.digital_read()) {
            // Aborted read
            this->bus_status_ = SiedleInhomeBus::idle;
            this->aborted_at_pin_ = 100 + this->bits_left_;
            return;
        }
        bool received_bit = this->isr_rx_pin_.digital_read();
        this->transferred_cmd_ = (this->transferred_cmd_<<1) + received_bit;

        this->bits_left_--;
        if (this->bits_left_ > 0) {
            // Wait for next bit
            this->bit_ticks_left_ = TICKS_PER_BIT;
        } else {
            //Transmission end
            this->bus_status_ = SiedleInhomeBus::received;
            last_command_complete_at_ = micros();
        }
        return;
    }

    case SiedleInhomeBus::sending: {
        if (this->bits_left_ > 0) {
            this->bits_left_--;
            bool bit_to_send = bitRead(this->transferred_cmd_, this->bits_left_);
            this->isr_tx_pin_.digital_write(bit_to_send);            
            // Wait for next bit
            this->bit_ticks_left_ = TICKS_PER_BIT;
        } else {
            // End transmission
            this->isr_tx_pin_.digital_write(true);
            this->isr_load_pin_.digital_write(false);
            this->bus_status_ = SiedleInhomeBus::idle;
            last_command_complete_at_ = micros();
        }
        return;
    }

    default:
        // Unexpected bus_status
        this->bus_status_ = SiedleInhomeBus::idle;
        return;
    }
}

void IRAM_ATTR HOT SiedleInhomeBus::s_timer_intr(SiedleInhomeBus *arg) {
    arg->timer_intr();
}

uint32_t SiedleInhomeBus::get_received_cmd() {
    if (!this->is_received_cmd()) {
        ESP_LOGE(TAG, "get_received_cmd called but not cmd received. bus_status_: %d", this->bus_status_);
    }

    auto cmd = this->transferred_cmd_;
    this->bus_status_ = SiedleInhomeBus::idle;
    return cmd;
}


}  // namespace siedle_inhome_bus
}  // namespace esphome