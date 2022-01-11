#include "vl53l1x_sensor.h"
#include "esphome/core/log.h"

extern "C" {
  #include "VL53L1X_api.h"
}
#include "esphome/components/i2c_platform/i2c_platform.h"

/*
 * Most of the code in this integration is based on the VL53L0x library
 * by Pololu (Pololu Corporation), which in turn is based on the VL53L0X
 * API from ST.
 *
 * For more information about licensing, please view the included LICENSE.txt file
 * in the vl53l1x integration directory.
 */

namespace esphome {
namespace vl53l1x {

static const char *const TAG = "vl53l1x";
static const uint8_t WAITS_FOR_DATA = 5;

std::list<VL53L1XSensor *> VL53L1XSensor::vl53_sensors;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
bool VL53L1XSensor::enable_pin_setup_complete = false;   // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

VL53L1XSensor::VL53L1XSensor() {
  VL53L1XSensor::vl53_sensors.push_back(this);
  handle = i2c_platform_register_component(this);
}

void VL53L1XSensor::dump_config() {
  LOG_SENSOR("", "VL53L1X", this);
  LOG_UPDATE_INTERVAL(this);
  LOG_I2C_DEVICE(this);
  if (this->enable_pin_ != nullptr) {
    LOG_PIN("  Enable Pin: ", this->enable_pin_);
  }
  if (this->interrupt_pin_ != nullptr) {
    LOG_PIN("  Interrupt Pin: ", this->interrupt_pin_);
  }
  if (this->speed_sensor_ != nullptr)        LOG_SENSOR("", "VL53L1X speed", this->speed_sensor_ );
  if (this->acceleration_sensor_ != nullptr) LOG_SENSOR("", "VL53L1X acceleration", this->acceleration_sensor_ );
  //ESP_LOGCONFIG(TAG, "  Timeout: %u%s", this->timeout_us_, this->timeout_us_ > 0 ? "us" : " (no timeout)");
}

void VL53L1XSensor::setup() {
  ESP_LOGD(TAG, "'%s' - setup BEGIN", this->name_.c_str());

  if (!esphome::vl53l1x::VL53L1XSensor::enable_pin_setup_complete) {
    for (auto &vl53_sensor : vl53_sensors) {
      if (vl53_sensor->enable_pin_ != nullptr) {
        // Set enable pin as OUTPUT and disable the enable pin to force vl53 to HW Standby mode
        vl53_sensor->enable_pin_->setup();
        vl53_sensor->enable_pin_->digital_write(false);
      }
    }
    esphome::vl53l1x::VL53L1XSensor::enable_pin_setup_complete = true;
  }

  if (this->enable_pin_ != nullptr) {
    // Enable the enable pin to cause FW boot (to get back to 0x29 default address)
    this->enable_pin_->setup();
    this->enable_pin_->digital_write(true);
    delayMicroseconds(100);
  }

  // Save the i2c address we want and force it to use the default 0x29
  // until we finish setup, then re-address to final desired address.
  uint8_t final_address = address_;
  this->set_i2c_address(0x29);

  //Check the sensor ID
  uint16_t sensorId = 0;
  uint8_t result = VL53L1X_GetSensorId(handle, &sensorId);
  if ( sensorId != 0xEACC ) {
    ESP_LOGE(TAG, "'%s' - Sensor connection failed (read: %X, ID: %X) - retry so error can be seen.", this->name_.c_str(), result, sensorId);
    /*manage here device not found */
    this->status_set_error();
    return;
  }
  
  // Set the sensor to the desired final address
  //TBD
  this->set_i2c_address(final_address);

  this->setup_ranging();

  ESP_LOGD(TAG, "'%s' - setup END", this->name_.c_str());
}

void VL53L1XSensor::setup_ranging() {
  ESP_LOGD(TAG, "'%s' - setup ranging BEGIN", this->name_.c_str());

  // Init sensor
  uint8_t status = VL53L1X_SensorInit(handle);

  /**
  * Set the distance mode (1=short, 2=long(default)).
  * Short mode max distance is limited to 1.3 m but better ambient immunity.\n
  * Long mode can range up to 4 m in the dark with 200 ms timing budget.
  */
  if (this->long_range_)
    status |= VL53L1X_SetDistanceMode(handle, 2);
  else
    status |= VL53L1X_SetDistanceMode(handle, 1);

  //Set the timing budget in ms.
  // Predefined values = 15, 20, 33, 50, 100(default), 200, 500.
  //TBD - set the closest but <=N get_update_interval()
  status |= VL53L1X_SetTimingBudgetInMs(handle, 200);

  // Intermeasurement period must be >= timing budget. Default = 100 ms.
  status |= VL53L1X_SetInterMeasurementInMs(handle, this->get_update_interval());

  status |= VL53L1X_StartRanging(handle); // Start only once (and do never call stop)

  // Set/clear error status
  if (status == 0) {
    this->status_clear_error();
    this->waitsForData = WAITS_FOR_DATA;
  } else {
    this->status_set_error();
  }

  ESP_LOGD(TAG, "'%s' - setup ranging END with status %d", this->name_.c_str(), status);
}

void VL53L1XSensor::update() {
  if (this->status_has_warning() or this->status_has_error()) {
    ESP_LOGW(TAG, "'%s' - Retrying to reconnect the sensor.", this->name_.c_str());
    this->setup_ranging();
    return;
  }
  
  uint8_t data_ready;
  uint16_t new_distance;
  uint8_t result;
  result = VL53L1X_CheckForDataReady(handle, &data_ready);
  if (result || !data_ready) {
    //quick retry
    delay(1);
    result = VL53L1X_CheckForDataReady(handle, &data_ready);
  }
  if (result || !data_ready) {
    ESP_LOGW(TAG, "'%s' - No data ready!", this->name_.c_str());
    if (this->waitsForData-- == 0) {
      this->setup_ranging();
    } 
    if (this->error_sensor_ != nullptr)  this->error_sensor_->publish_state(WAITS_FOR_DATA - this->waitsForData);
    return;
  }

  //Reset wait counter
  if (this->waitsForData != WAITS_FOR_DATA) {
    this->waitsForData = WAITS_FOR_DATA;
    if (this->error_sensor_ != nullptr)  this->error_sensor_->publish_state(0);
  }

  result = VL53L1X_GetDistance(handle, &new_distance);
  result |= VL53L1X_ClearInterrupt(handle);
  if (result) {
    ESP_LOGW(TAG, "'%s' - Error reading! result: %d", this->name_.c_str(), result);
    this->setup_ranging();
    return;
  }

  //avoid initial jump of speed
  if (this->distance_ == 0) {
    this->distance_ = new_distance;
  }

  //calculate new speed and acceleration
  auto update_interval = this->get_update_interval();
  float new_speed = ((float)new_distance - this->distance_) / update_interval;
  float new_acceleration = 1000 * (new_speed - this->speed_) / update_interval;
  
  //publish distance
  this->distance_ = new_distance;
  this->publish_state(new_distance);

  //publish speed
  this->speed_ = new_speed;
  if (this->speed_sensor_ != nullptr)        this->speed_sensor_->publish_state(new_speed);

  //publish acceleration
  this->acceleration_ = new_acceleration;
  if (this->acceleration_sensor_ != nullptr) this->acceleration_sensor_->publish_state(new_acceleration);

}

}  // namespace vl53l1x
}  // namespace esphome