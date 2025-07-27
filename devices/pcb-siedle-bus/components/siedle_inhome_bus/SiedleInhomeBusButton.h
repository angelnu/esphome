#pragma once

#include "esphome/core/defines.h"
#include "esphome/components/button/button.h"
#include "esphome/core/component.h"
#include "SiedleInhomeBus.h"

namespace esphome {
namespace siedle_inhome_bus {

class SiedleInhomeBusButton : public button::Button, public Component {
 public:
  void set_parent(SiedleInhomeBus *parent) { parent_ = parent; }
  void set_command(uint8_t command,
                   uint8_t destination,
                   uint8_t destination_bus,
                   uint8_t source,
                   uint8_t source_bus) {
    command_ = new SiedleInhomeBusCommand(command, destination, destination_bus, source, source_bus);
  }

  void dump_config() override;

 protected:
  void press_action() override;

  SiedleInhomeBus *parent_;
  SiedleInhomeBusCommand *command_;
};

}  // namespace siedle_inhome_bus
}  // namespace esphome