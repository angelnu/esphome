#include "vl53l1x_pololu.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace vl53l1x_pololu {

static const char *const TAG = "vl53l1x_pololu";

// No fresh ranging frame for this long => sensor hung => power-cycle + re-init.
static const uint32_t FREEZE_TIMEOUT_MS = 5000;

bool VL53L1XPololuComponent::start_ranging_() {
  if (!this->sensor_.setDistanceMode((VL53L1X::DistanceMode) this->distance_mode_)) {
    ESP_LOGE(TAG, "setDistanceMode failed");
    return false;
  }
  if (!this->sensor_.setMeasurementTimingBudget(this->timing_budget_us_)) {
    ESP_LOGE(TAG, "setMeasurementTimingBudget(%u us) failed", (unsigned) this->timing_budget_us_);
    return false;
  }
  this->sensor_.startContinuous(this->inter_measurement_ms_);
  return true;
}

void VL53L1XPololuComponent::setup() {
  // XSHUT: release the sensor from reset (boards that wire it to a GPIO).
  if (this->enable_pin_ != nullptr) {
    this->enable_pin_->setup();
    this->enable_pin_->digital_write(true);
    delay(2);
  }
  this->sensor_.set_i2c(this);
  this->sensor_.setTimeout(500);
  if (!this->sensor_.init()) {
    ESP_LOGE(TAG, "Pololu VL53L1X init() failed (model id / boot)");
    this->mark_failed();
    return;
  }
  if (!this->start_ranging_()) {
    this->mark_failed();
    return;
  }
  this->last_frame_ms_ = millis();
  this->ok_ = true;
}

void VL53L1XPololuComponent::read_frame_() {
  const uint32_t now = millis();
  const uint16_t d = this->sensor_.read(false);  // reads results + clears interrupt
  this->last_status_ = this->sensor_.ranging_data.range_status;
  this->frame_count_++;
  this->last_frame_ms_ = now;

  // Speed/acceleration from consecutive frames over the real elapsed time.
  if (this->speed_have_last_) {
    const float dt_ms = (float) (now - this->speed_last_ms_);
    if (dt_ms > 0.0f) {
      const float new_speed = ((float) d - (float) this->speed_last_dist_) / dt_ms;  // mm/ms == m/s
      const float new_accel = 1000.0f * (new_speed - this->speed_) / dt_ms;          // m/s^2
      this->speed_ = new_speed;
      this->acceleration_ = new_accel;
      if (this->speed_sensor_ != nullptr)
        this->speed_sensor_->publish_state(new_speed);
      if (this->acceleration_sensor_ != nullptr)
        this->acceleration_sensor_->publish_state(new_accel);
    }
  }
  this->speed_last_dist_ = d;
  this->speed_last_ms_ = now;
  this->speed_have_last_ = true;
  this->last_distance_ = d;
}

void VL53L1XPololuComponent::recover_() {
  // Power-cycle the sensor when XSHUT is wired: releases a locked bus and boots clean.
  if (this->enable_pin_ != nullptr) {
    this->enable_pin_->digital_write(false);
    delay(3);
    this->enable_pin_->digital_write(true);
    delay(3);
  } else {
    this->sensor_.stopContinuous();  // best effort
  }
  if (this->sensor_.init() && this->start_ranging_()) {
    this->recovery_count_++;
    if (this->recovery_count_sensor_ != nullptr)
      this->recovery_count_sensor_->publish_state(this->recovery_count_);
    ESP_LOGW(TAG, "Sensor recovered (total recoveries: %u)", (unsigned) this->recovery_count_);
  } else {
    ESP_LOGE(TAG, "Re-init failed; will retry");
  }
  this->speed_have_last_ = false;  // reset the speed baseline after a reset
}

void VL53L1XPololuComponent::update() {
  if (!this->ok_)
    return;

  // One frame per polling cycle (see the header note about loop() flooding).
  if (this->sensor_.dataReady()) {
    this->read_frame_();
  } else if (millis() - this->last_frame_ms_ > FREEZE_TIMEOUT_MS) {
    ESP_LOGW(TAG, "No ranging frame for %ums — re-initialising sensor",
             (unsigned) (millis() - this->last_frame_ms_));
    this->recover_();
    this->last_frame_ms_ = millis();
  }

  if (this->distance_sensor_ != nullptr)
    this->distance_sensor_->publish_state(this->last_distance_);
  if (this->range_status_sensor_ != nullptr)
    this->range_status_sensor_->publish_state(this->sensor_.ranging_data.range_status);
  if (this->signal_rate_sensor_ != nullptr)
    this->signal_rate_sensor_->publish_state(this->sensor_.ranging_data.peak_signal_count_rate_MCPS);
  if (this->ambient_rate_sensor_ != nullptr)
    this->ambient_rate_sensor_->publish_state(this->sensor_.ranging_data.ambient_count_rate_MCPS);
  if (this->frames_sensor_ != nullptr)
    this->frames_sensor_->publish_state(this->frame_count_);
  if (this->recovery_count_sensor_ != nullptr)
    this->recovery_count_sensor_->publish_state(this->recovery_count_);
}

void VL53L1XPololuComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "VL53L1X (Pololu port), continuous ranging:");
  ESP_LOGCONFIG(TAG, "  Distance mode: %u (0=short,1=medium,2=long)", this->distance_mode_);
  ESP_LOGCONFIG(TAG, "  Timing budget: %u us", (unsigned) this->timing_budget_us_);
  ESP_LOGCONFIG(TAG, "  Inter-measurement: %u ms", (unsigned) this->inter_measurement_ms_);
  LOG_I2C_DEVICE(this);
  if (this->enable_pin_ != nullptr)
    LOG_PIN("  XSHUT Pin: ", this->enable_pin_);
  if (this->is_failed())
    ESP_LOGE(TAG, "  init failed");
}

}  // namespace vl53l1x_pololu
}  // namespace esphome
