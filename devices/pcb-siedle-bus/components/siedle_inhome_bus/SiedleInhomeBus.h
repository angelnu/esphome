#pragma once

#include <map>
#include <queue>

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <esp32-hal-timer.h>

#include "SiedleInhomeBusCommand.h"

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
                              uint8_t command,
                              uint8_t destination,
                              uint8_t destination_bus,
                              uint8_t source,
                              uint8_t source_bus) {
    auto cmd = new SiedleInhomeBusCommand(command, destination, destination_bus, source, source_bus);
    uint32_t cmd_raw = cmd -> get_raw();
    binary_sensors_commands_[cmd_raw] = cmd;
    this->binary_sensors_[cmd_raw] = obj;
  }

  void send_cmd (SiedleInhomeBusCommand& cmd) { this->to_be_send_cmds_.push(cmd.get_raw()); }

 protected:
  // Bus settings
  InternalGPIOPin *carrier_pin_;
  InternalGPIOPin *rx_pin_;
  InternalGPIOPin *load_pin_;
  InternalGPIOPin *tx_pin_;
  bool enabled_dump_;

  // Binary sensors
  std::map<uint32_t, binary_sensor::BinarySensor *> binary_sensors_;
  std::map<uint32_t, SiedleInhomeBusCommand *> binary_sensors_commands_;

  // Command queues
  std::queue<SiedleInhomeBusCommand> received_comands_;
  std::queue<SiedleInhomeBusCommand> to_be_send_cmds_;
  
  bool is_received_cmd() { return !this->received_comands_.empty(); }
  SiedleInhomeBusCommand get_received_cmd();
  
  bool is_to_be_send_cmd() { return !this->to_be_send_cmds_.empty(); }
  SiedleInhomeBusCommand get_to_be_send_cmd();
  void send_cmd_internal (SiedleInhomeBusCommand&);

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