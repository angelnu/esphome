#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/stepper/stepper.h"
#include "esphome/components/uart/uart.h"
//Stream wrapper for esphome
#include "esphome_stream.h"
//Stepper library for TMC chipsets
#include "TMCStepper.h"

#include <limits> 

namespace esphome {
namespace tmc2209 {

class TMC2209 : public stepper::Stepper, public uart::UARTDevice, public Component {
 public:
  void set_r_sense(float r_sense) { r_sense_ = r_sense; }
  void set_freewheeling_mode(uint8_t freewheeling_mode) { freewheeling_mode_ = freewheeling_mode; }
  void set_inverted(bool inverted) { inverted_ = inverted; }
  void set_enable_pin(GPIOPin *enable_pin) { enable_pin_ = enable_pin; }
  void set_index_pin(InternalGPIOPin *index_pin) { index_pin_ = index_pin; }
  void set_diag_pin(GPIOPin *diag_pin) { diag_pin_ = diag_pin; }
  void set_step_pin(GPIOPin *step_pin) { step_pin_ = step_pin; }
  void set_dir_pin(GPIOPin *dir_pin) { dir_pin_ = dir_pin; }
  void set_driver_address(uint8_t driver_address) { driver_address_ = driver_address; }
  void set_stall_threshold(uint8_t stall_threshold) { stall_threshold_ = stall_threshold; }
  void set_microsteps(uint16_t microsteps) { microsteps_ = microsteps; }
  void set_rms_current(float rms_current) { rms_current_ = rms_current; }

  void setup() override;
  void dump_config() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void    set_min_step(int32_t min_step) { min_step_ = min_step; }
  int32_t get_min_step()                 { return      min_step_;}
  void    set_max_step(int32_t max_step) { max_step_ = max_step; }
  int32_t get_max_step()                 { return      max_step_;}

  void calibrate();
  bool is_calibrating() { return is_calibrating_; }

 protected:
  static void index_gpio_intr(TMC2209 *stepper);
  volatile uint32_t index_counter_=0;

  bool get_dir() { return (this->target_position > this->current_position); } // true -> advance, false -> retrocede 

  float r_sense_;
  uint8_t freewheeling_mode_{0};
  bool inverted_{false};
  GPIOPin *enable_pin_{nullptr};
  InternalGPIOPin *index_pin_{nullptr};
  ISRInternalGPIOPin index_isr_pin_{nullptr};
  GPIOPin *diag_pin_{nullptr};
  GPIOPin *step_pin_{nullptr};
  GPIOPin *dir_pin_{nullptr};
  uint8_t driver_address_{0};
  uint8_t stall_threshold_{0};
  uint16_t microsteps_{0};
  float rms_current_{1.0};

  int32_t min_step_ = 0; //std::numeric_limits<int32_t>::min();
  int32_t max_step_ =  2100; //std::numeric_limits<int32_t>::max();
  bool is_calibrating_ = false;
  enum is_stallguard_status_t{DISSABLED, ENABLING, ENABLED};
  is_stallguard_status_t is_stallguard_status = DISSABLED;
  
  HighFrequencyLoopRequester high_freq_;

  TMC2209Stepper *driver_ptr_{nullptr};
  ESPHome_Stream *stream_{nullptr};
};

template<typename... Ts> class CalibrateAction : public Action<Ts...> {
 public:
  explicit CalibrateAction(TMC2209 *parent) : parent_(parent) {}

  void play(Ts... x) override {
    this->parent_->calibrate();
  }

 protected:
  TMC2209 *parent_;
};

}  // namespace tmc2209
}  // namespace esphome