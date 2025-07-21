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
  void set_command(uint32_t command) { command_ = command; }

  void dump_config() override;

 protected:
  void press_action() override;

  SiedleInhomeBus *parent_;
  uint32_t command_;
};

}  // namespace siedle_inhome_bus
}  // namespace esphome