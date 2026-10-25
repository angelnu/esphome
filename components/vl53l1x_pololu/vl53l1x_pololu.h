#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "vl53l1x_core.h"

namespace esphome {
namespace vl53l1x_pololu {

// ESPHome wrapper around the vendored Pololu VL53L1X driver, driving the sensor
// in CONTINUOUS (autonomous) mode.
//
// Reading is done once per polling cycle in update() (NOT in loop()): polling
// dataReady() every loop iteration floods the I2C bus and locks it up on slow /
// marginal buses (e.g. the trashcan's 10 kHz door bus). One read per
// update_interval keeps the traffic comparable to the sensor's frame rate.
//
// Freeze recovery power-cycles the sensor over XSHUT (when wired): that releases
// a bus the VL53L1X has locked (SDA held) and gives a clean boot.
class VL53L1XPololuComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_distance_sensor(sensor::Sensor *s) { distance_sensor_ = s; }
  void set_range_status_sensor(sensor::Sensor *s) { range_status_sensor_ = s; }
  void set_signal_rate_sensor(sensor::Sensor *s) { signal_rate_sensor_ = s; }
  void set_ambient_rate_sensor(sensor::Sensor *s) { ambient_rate_sensor_ = s; }
  void set_frames_sensor(sensor::Sensor *s) { frames_sensor_ = s; }
  void set_recovery_count_sensor(sensor::Sensor *s) { recovery_count_sensor_ = s; }
  void set_speed_sensor(sensor::Sensor *s) { speed_sensor_ = s; }
  void set_acceleration_sensor(sensor::Sensor *s) { acceleration_sensor_ = s; }

  void set_enable_pin(GPIOPin *pin) { enable_pin_ = pin; }  // XSHUT (held high at boot)
  // Latest distance in mm, updated every poll cycle. Used by the trashcan logic
  // to get a FRESH bucket position (the driver INDEX count drifts when the
  // bucket is moved by hand).
  float get_distance() const { return (float) this->last_distance_; }
  // Range status of the last frame (0 = valid; see VL53L1X::RangeStatus).
  uint8_t get_range_status() const { return this->last_status_; }
  void set_distance_mode(uint8_t mode) { distance_mode_ = mode; }            // 0=short,1=medium,2=long
  void set_timing_budget_us(uint32_t us) { timing_budget_us_ = us; }
  void set_inter_measurement_ms(uint32_t ms) { inter_measurement_ms_ = ms; }

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  bool start_ranging_();  // setDistanceMode + setMeasurementTimingBudget + startContinuous
  void read_frame_();     // read one frame + derive speed/acceleration
  void recover_();        // power-cycle (XSHUT) + re-init

  VL53L1X sensor_;
  GPIOPin *enable_pin_{nullptr};

  sensor::Sensor *distance_sensor_{nullptr};
  sensor::Sensor *range_status_sensor_{nullptr};
  sensor::Sensor *signal_rate_sensor_{nullptr};
  sensor::Sensor *ambient_rate_sensor_{nullptr};
  sensor::Sensor *frames_sensor_{nullptr};
  sensor::Sensor *recovery_count_sensor_{nullptr};
  sensor::Sensor *speed_sensor_{nullptr};
  sensor::Sensor *acceleration_sensor_{nullptr};

  uint8_t distance_mode_{VL53L1X::Long};
  uint32_t timing_budget_us_{50000};
  uint32_t inter_measurement_ms_{100};

  uint16_t last_distance_{0};
  uint8_t last_status_{255};
  uint32_t frame_count_{0};
  uint32_t recovery_count_{0};
  uint32_t last_frame_ms_{0};
  bool ok_{false};

  // Derived speed/acceleration, computed from consecutive frames (mm/ms == m/s).
  float speed_{0.0f};
  float acceleration_{0.0f};
  uint16_t speed_last_dist_{0};
  uint32_t speed_last_ms_{0};
  bool speed_have_last_{false};
};

}  // namespace vl53l1x_pololu
}  // namespace esphome
