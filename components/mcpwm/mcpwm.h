#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/output/float_output.h"

#include <driver/mcpwm_prelude.h>

#ifdef USE_ESP32

namespace esphome {
namespace mcpwm {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern uint8_t next_mcpwm_channel;

class MCPWMOutput : public output::FloatOutput, public Component {
 public:
  explicit MCPWMOutput(InternalGPIOPin *pin) : pin_(pin) { this->channel_ = next_mcpwm_channel++; }

  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_frequency(float frequency) { this->frequency_ = frequency; }

  /// Setup MCPWM.
  void setup() override;
  void dump_config() override;
  /// HARDWARE setup priority
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  /// Override FloatOutput's write_state.
  void write_state(float state) override;

 protected:
  InternalGPIOPin *pin_;
  uint8_t channel_{};
  float frequency_{};
  float duty_{0.0f};
  bool initialized_ = false;

  mcpwm_cmpr_handle_t comparator_ = NULL;
};

}  // namespace mcpwm
}  // namespace esphome

#endif