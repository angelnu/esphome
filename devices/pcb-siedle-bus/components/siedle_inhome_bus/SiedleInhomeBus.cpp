#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "SiedleInhomeBus.h"


namespace esphome {
namespace siedle_inhome_bus {

static const char *TAG = "siedle_inhome_bus.component";

static const uint32_t BIT_DURATION_US = 1980;
static const uint8_t TICKS_PER_BIT = 4;

void SiedleInhomeBus::setup() {
    this->carrier_pin_->setup();
    this->rx_pin_->setup();
    this->load_pin_->setup();
    this->tx_pin_->setup();

    auto input_interrupt_pin_ = this->carrier_pin_->to_isr();
    this->carrier_pin_->attach_interrupt(&SiedleInhomeBus::s_gpio_intr, this,
                                         gpio::INTERRUPT_RISING_EDGE);
    
    this -> bus_timer_ = timerBegin(1000000);
    timerAttachInterrupt(bus_timer_, &SiedleInhomeBus::s_timer_intr);
    // For ESP32, we can't use dynamic interval calculation because the timerX functions
    // are not callable from ISR (placed in flash storage).
    // Here we just use an interrupt firing every 50 µs.
    timerAlarm(bus_timer, BIT_DURATION_US / TICKS_PER_BIT, true, 0);
}

void SiedleInhomeBus::loop() {
    if (this->last_gpio_interrupt_at_ != 0) {
        ESP_LOGI(TAG, "Interrupt received at %" PRIu32, this->last_gpio_interrupt_at_);
        this->last_gpio_interrupt_at_ = 0;
    }

    if (is_received_cmd()) {
        auto cmd = get_received_cmd();
        ESP_LOGI (TAG, "Received CMD: %s", format_hex_pretty(cmd));
    }

}

void SiedleInhomeBus::dump_config(){
    ESP_LOGCONFIG(TAG, "Siedle In-Home Bus");

    LOG_PIN("  Carrier Pin: ", this->carrier_pin_);
    LOG_PIN("  RX Pin:      ", this->rx_pin_);
    LOG_PIN("  Load Pin:    ", this->load_pin_);
    LOG_PIN("  TX Pin:      ", this->tx_pin_);

    ESP_LOGI(TAG, "Carrier: %d Bits: %d", this->carrier_pin_->digital_read(), this->rx_pin_->digital_read());
}


/// GPIO interrupt routine, called when incoming msg
void IRAM_ATTR HOT SiedleInhomeBus::gpio_intr() {
    this->last_gpio_interrupt_at_ = micros();

    //Start read
    if (this->bus_status_ != SiedleInhomeBus::idle) {
        //Cannot read new cmd if not idle
        return;
    }

    this->bus_status_ != SiedleInhomeBus::receiving;

    this->bit_ticks_left_ = TICKS_PER_BIT / 2;
    this->bits_left_ = 32;

}

void IRAM_ATTR HOT SiedleInhomeBus::s_gpio_intr(SiedleInhomeBus *arg) {
    arg->gpio_intr();
}


void SiedleInhomeBus::send_cmd(uint32_t cmd) {
    if (this->bus_status_ != SiedleInhomeBus::idle) {
        //Cannot transfer new cmd if not idle
        return;
    }

    this->bus_status_ != SiedleInhomeBus::sending;
    this->transferred_cmd_ = cmd;

    // Enable load to write
    this->load_pin_->digital_write(true);
    this->tx_pin_->digital_write(this->transferred_cmd_ >> 31);

    this->bit_ticks_left_ = TICKS_PER_BIT;
    this->bits_left_ = 31;
}

/// Timmer interrupt routine, called each BIT_DURATION_US / TICKS_PER_BIT
void IRAM_ATTR HOT SiedleInhomeBus::timer_intr() {

    if (this->bus_status_ == SiedleInhomeBus::receiving || this->bus_status_ == SiedleInhomeBus::sending) {
        return;
    }

    this->bit_ticks_left_--;
    if ( this->bit_ticks_left_ > 0) {
        return;
    }
    
    switch (this->bus_status_)
    {
    case SiedleInhomeBus::receiving: {
        if (!this->carrier_pin_->digital_read()) {
            // Aborted read
            this->bus_status_ = SiedleInhomeBus::idle;
        }
        bool received_bit = !this->rx_pin_->digital_read();
        this->transferred_cmd_ = (this->transferred_cmd_<<1) + received_bit;

        this->bits_left_--;
        if (this->bits_left_ > 0) {
            // Wait for next bit
            this->bit_ticks_left_ = TICKS_PER_BIT;
        } else {
            //Transmission end
            this->bus_status_ = SiedleInhomeBus::received;
        }
        return;
    }

    case SiedleInhomeBus::sending: {
        if (this->bits_left_ > 0) {
            this->bits_left_--;
            bool bit_to_send = (this->transferred_cmd_ << this->bits_left_) | 1;
            this->tx_pin_->digital_write(!bit_to_send);
        } else {
            // End transmission
            this->load_pin_->digital_write(false);
            this->bus_status_ != SiedleInhomeBus::idle;
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