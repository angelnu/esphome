#include "stepper_cover.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <cfloat>

namespace esphome {
namespace tmc2209 {

static const char *const TAG = "tmc2209.cover";

using namespace esphome::cover;

void StepperCover::setup() {
  ESP_LOGCONFIG(TAG, "Setting up stepper cover ...");
}

void StepperCover::control(const CoverCall &call) {
  if (call.get_stop()) {
    this->stepper_->set_target(this->stepper_->current_position);
  }
  if (call.get_position().has_value()) {
    auto pos = *call.get_position();
    this->stepper_->set_target((pos * this->get_stepper_range()) + this->stepper_->get_min_step());
  }
}

void StepperCover::update() {
  
  bool changed = false;

  auto old_position = this->position;
  this->position = (float)(this->stepper_->current_position - this->stepper_->get_min_step()) / this->get_stepper_range();
  changed |= (old_position != this->position);

  auto old_operation = this->current_operation;
  if (this->stepper_->current_position < this->stepper_->target_position)
    this->current_operation = COVER_OPERATION_OPENING;
  else if (this->stepper_->current_position > this->stepper_->target_position)
    this->current_operation = COVER_OPERATION_CLOSING;
  else
    this->current_operation = COVER_OPERATION_IDLE;
  changed |= (old_operation != this->current_operation);

  if (changed) this->publish_state(false);
}

void StepperCover::dump_config() {
  LOG_COVER("", "stepper", this);
}

}  // namespace current_based
}  // namespace esphome