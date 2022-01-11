#pragma once

#include <list>

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"


namespace esphome {
namespace vl53l1x {

class VL53L1XSensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 public:
  VL53L1XSensor();

  void setup() override;

  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void update() override;

  //void loop() override;

  void setup_ranging();
  void set_signal_rate_limit(float signal_rate_limit) { signal_rate_limit_ = signal_rate_limit; }
  void set_long_range(bool long_range) { long_range_ = long_range; }
  void set_enable_pin(GPIOPin *enable) { this->enable_pin_ = enable; }
  void set_interrupt_pin(GPIOPin *interrupt) { this->interrupt_pin_ = interrupt; }

  void set_speed_sensor(sensor::Sensor *speed_sensor) { speed_sensor_ = speed_sensor; }
  void set_acceleration_sensor(sensor::Sensor *acceleration_sensor) { acceleration_sensor_ = acceleration_sensor; }
  void set_error_sensor(sensor::Sensor *error_sensor) { error_sensor_ = error_sensor; }

 protected:

  float signal_rate_limit_;
  bool long_range_;
  GPIOPin *enable_pin_{nullptr};
  GPIOPin *interrupt_pin_{nullptr};
  sensor::Sensor *speed_sensor_{nullptr};
  sensor::Sensor *acceleration_sensor_{nullptr};
  sensor::Sensor *error_sensor_{nullptr};

  uint8_t handle;
  uint16_t distance_{0};
  float speed_{0};
  float acceleration_{0};
  uint8_t waitsForData{0};

  static std::list<VL53L1XSensor *> vl53_sensors;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  static bool enable_pin_setup_complete;           // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};

}  // namespace vl53l1x
}  // namespace esphome