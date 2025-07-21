#pragma once

#include <map>

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <esp32-hal-timer.h>

namespace esphome {
namespace siedle_inhome_bus {

class SiedleInhomeBus : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_carrier_pin(InternalGPIOPin *carrier_pin) { carrier_pin_ =  carrier_pin; }
  void set_rx_pin     (InternalGPIOPin *rx_pin)      { rx_pin_       = rx_pin; }
  void set_load_pin   (InternalGPIOPin *load_pin)    { load_pin_     = load_pin; }
  void set_tx_pin     (InternalGPIOPin *tx_pin)      { tx_pin_       = tx_pin; }
  void set_dump       (bool enabled_dump)            { enabled_dump_ = enabled_dump;}

  void register_binary_sensor(uint32_t command, binary_sensor::BinarySensor *obj) { this->binary_sensors_[command] = obj; }

  void send_cmd (uint32_t);
  
  bool is_received_cmd() { return this->bus_status_ == SiedleInhomeBus::received; }
  uint32_t get_received_cmd();

 protected:
  // Bus settings
  InternalGPIOPin *carrier_pin_;
  InternalGPIOPin *rx_pin_;
  InternalGPIOPin *load_pin_;
  InternalGPIOPin *tx_pin_;
  bool enabled_dump_;

  // Binary sensors
  std::map<uint32_t, binary_sensor::BinarySensor *> binary_sensors_;

  //Interrupt pins
  ISRInternalGPIOPin isr_carrier_pin_;
  ISRInternalGPIOPin isr_rx_pin_;
  ISRInternalGPIOPin isr_load_pin_;
  ISRInternalGPIOPin isr_tx_pin_;

  // Last interrupt time (in micros())
  uint32_t last_gpio_interrupt_at_;
  uint32_t last_command_complete_at_=0;
  hw_timer_t *bus_timer_;

  enum bus_status_t {idle, receiving, received, sending};

  bus_status_t bus_status_ = idle;
  uint32_t transferred_cmd_;
  uint8_t bits_left_;
  uint16_t bit_ticks_left_;

  static void s_gpio_intr(SiedleInhomeBus *arg);
  void gpio_intr();

  static void s_timer_intr(SiedleInhomeBus *arg);
  void timer_intr();
};

}  // namespace siedle_inhome_bus
}  // namespace esphome