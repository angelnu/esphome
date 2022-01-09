#pragma once

#include "esphome/components/cover/cover.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include <cfloat>

#include "tmc2209.h"

namespace esphome {
namespace tmc2209 {

class StepperCover : public cover::Cover, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  cover::CoverTraits get_traits() override {
    auto traits = cover::CoverTraits();
    traits.set_supports_position(true);
    traits.set_is_assumed_state(false);
    return traits;
  }

  void set_stepper(TMC2209 *stepper) { stepper_ = stepper; }

 protected:
  void control(const cover::CoverCall &call) override;

  uint32_t get_stepper_range() { return this-stepper_->get_max_step() - this-stepper_->get_min_step(); }

  TMC2209* stepper_;
};

}  // namespace current_based
}  // namespace esphome