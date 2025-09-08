#include "mcpwm.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

static const uint8_t SETUP_ATTEMPT_COUNT_MAX = 5;

static const size_t TIMEBASE_RESOLUTION_HZ = 1000000;  // 1MHz, 1us per tick
static const size_t MIN_PULSEWIDTH_US = 500;  // Minimum pulse width in microsecond
static const size_t MAX_PULSEWIDTH_US = 2500;  // Maximum pulse width in microsecond

namespace esphome {
namespace mcpwm {

static const char *const TAG = "mcpwm.output";

constexpr int mcpwm_ticks_period(float frequency) {
  return static_cast<uint32_t>(TIMEBASE_RESOLUTION_HZ / frequency);
}

void MCPWMOutput::write_state(float state) {
  if (!this->initialized_) {
    ESP_LOGW(TAG, "Not yet initialized");
    return;
  }

  if (this->pin_->is_inverted())
    state = 1.0f - state;

  this->duty_ = state;
  
  ESP_LOGV(TAG, "Setting duty: %f on channel %u", this->duty_, this->channel_);

  ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator_, static_cast<uint32_t>(duty_ * mcpwm_ticks_period(frequency_))));
}

void MCPWMOutput::setup() {
  mcpwm_timer_handle_t timer = NULL;
  mcpwm_timer_config_t timer_config = {
      .group_id = 0,  // TBD: select right timer and channel
      .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
      .resolution_hz = TIMEBASE_RESOLUTION_HZ,
      .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
  };
  timer_config.period_ticks = mcpwm_ticks_period(frequency_);
  ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

  mcpwm_oper_handle_t oper = NULL;
  mcpwm_operator_config_t operator_config = {
      .group_id = 0, // operator must be in the same group to the timer
  };
  ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

  ESP_LOGI(TAG, "Connect timer and operator");
  ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

  ESP_LOGI(TAG, "Create comparator and generator from the operator");
  mcpwm_comparator_config_t comparator_config = {};
  comparator_config.flags.update_cmp_on_tez = true;
  ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator_));

  mcpwm_gen_handle_t generator = NULL;
  mcpwm_generator_config_t generator_config = {
      .gen_gpio_num = this->pin_->get_pin(),
  };
  ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

  // set the initial compare value to 0
  ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator_, 0));

  ESP_LOGI(TAG, "Set generator action on timer and compare event");
  // go high on counter empty
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
  // go low on compare threshold
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                              MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_, MCPWM_GEN_ACTION_LOW)));

  ESP_LOGI(TAG, "Enable and start timer");
  ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
  ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
  
  this->initialized_ = true;
  this->status_clear_error();
}

void MCPWMOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "Output:");
  LOG_PIN("  Pin ", this->pin_);
  ESP_LOGCONFIG(TAG,
                "  Channel: %u\n"
                "  PWM Frequency: %.1f Hz\n",
                this->channel_, this->frequency_);
}

uint8_t next_mcpwm_channel = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace mcpwm
}  // namespace esphome

#endif