#pragma once

#include <map>
#include <queue>

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <esp32-hal-timer.h>

#include "SiedleInhomeBusMessage.h"

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

  void register_binary_sensor(binary_sensor::BinarySensor *obj,
                              SiedleInhomeBusMessage *msg) {
    uint32_t msg_raw = msg -> get_raw();
    binary_sensors_messages_[msg_raw] = msg;
    this->binary_sensors_[msg_raw] = obj;
  }

  void send_message (SiedleInhomeBusMessage& msg) { this->to_be_send_messages_.push(msg.get_raw()); }

 protected:
  // Bus settings
  InternalGPIOPin *carrier_pin_;
  InternalGPIOPin *rx_pin_;
  InternalGPIOPin *load_pin_;
  InternalGPIOPin *tx_pin_;
  bool enabled_dump_;

  // Binary sensors
  std::map<uint32_t, binary_sensor::BinarySensor *> binary_sensors_;
  std::map<uint32_t, SiedleInhomeBusMessage *> binary_sensors_messages_;

  // Command queues
  std::queue<SiedleInhomeBusMessage> received_messages_;
  std::queue<SiedleInhomeBusMessage> to_be_send_messages_;
  
  bool is_received_messages() { return !this->received_messages_.empty(); }
  SiedleInhomeBusMessage get_received_message();
  
  bool is_to_be_send_messages() { return !this->to_be_send_messages_.empty(); }
  SiedleInhomeBusMessage get_to_be_send_message();
  void internal_send_message (SiedleInhomeBusMessage& msg);

  //Interrupt pins
  ISRInternalGPIOPin isr_carrier_pin_;
  ISRInternalGPIOPin isr_rx_pin_;
  ISRInternalGPIOPin isr_load_pin_;
  ISRInternalGPIOPin isr_tx_pin_;

  // Last interrupt time (in micros())
  uint32_t last_gpio_interrupt_at_=0;
  uint32_t last_command_complete_at_=0;
  uint8_t  aborted_at_pin_=0;
  hw_timer_t *bus_timer_;

  enum bus_status_t {idle, receiving, sending};

  bus_status_t bus_status_ = idle;
  uint32_t transferred_msg_;
  uint8_t bits_left_;
  uint16_t bit_ticks_left_;

  static void s_gpio_intr(SiedleInhomeBus *arg);
  void gpio_intr();

  static void s_timer_intr(SiedleInhomeBus *arg);
  void timer_intr();
};

}  // namespace siedle_inhome_bus
}  // namespace esphome