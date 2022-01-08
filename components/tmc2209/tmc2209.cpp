#include "tmc2209.h"
#include "esphome/core/log.h"


#define USE_VACTUAL 1

namespace esphome {
namespace tmc2209 {

static const char *const TAG = "tmc2209.stepper";
static const uint8_t TOFF_VALUE = 4;  //[1... 15]
DRAM_ATTR static const uint8_t STEPS_PER_INDEX = 4; // Index pin generates a pulse each 4 full steps

// This is needed if we use VACTUAL to convert from steps/t to steps/second
// multiply steps/seconds by MSTEP_T to get desired VACTUAL
static const uint32_t CLOCK_FREQ= 12*1000000;
static const double MSTEP_T = (double)(1<<24)/CLOCK_FREQ;
//static const double MSTEP_T=1.39810133333333000000;

void TMC2209::setup() {
  ESP_LOGCONFIG(TAG, "Setting up TMC2209...");

  //enable pin
  this->enable_pin_->setup();
  this->enable_pin_->digital_write(false);
  //index pin
  if (this->index_pin_) {
    this->index_isr_pin_ = index_pin_->to_isr();
    this->index_pin_->attach_interrupt(TMC2209::index_gpio_intr, this, gpio::INTERRUPT_RISING_EDGE); //TBD - use only rising edges
  }
  //diag pin
  if (this->diag_pin_) {
    this->diag_pin_->setup();
  }
  //step pin
  if (this->step_pin_) {
    this->step_pin_->setup();
    this->step_pin_->digital_write(false);
  }
  //dir pin
  if (this->dir_pin_) {
    this->dir_pin_->setup();
    this->dir_pin_->digital_write(false);
  }

  this->stream_ = new ESPHome_Stream(this);
  this->driver_ptr_ = new TMC2209Stepper(this->stream_, this->r_sense_, this->driver_address_);

  driver_ptr_->begin();

  // Sets the slow decay time (off time) [1... 15]. This setting also limits
  // the maximum chopper frequency. For operation with StealthChop,
  // this parameter is not used, but it is required to enable the motor.
  // In case of operation with StealthChop only, any setting is OK.
  driver_ptr_->toff(TOFF_VALUE);

  driver_ptr_->rms_current(this->rms_current_ * 1000); // mA
  
  // microsteps per step - valid values are 0 (no microsteps), 2, 4, 8, 16, 32, 64, 128, 256
  driver_ptr_->microsteps(this->microsteps_);

  // VACTUAL allows moving the motor by UART control.
  // It gives the motor velocity in +-(2^23)-1 [μsteps / t]
  // 0: Normal operation. Driver reacts to STEP input.
  // /=0: Motor moves with the velocity given by VACTUAL. 
  // Step pulses can be monitored via INDEX output.
  // The motor direction is controlled by the sign of VACTUAL.
  driver_ptr_->VACTUAL(0);

  // Comparator blank time. This time needs to safely cover the switching
  // event and the duration of the ringing on the sense resistor. For most
  // applications, a setting of 16 or 24 is good. For highly capacitive
  // loads, a setting of 32 or 40 will be required.
  driver_ptr_->blank_time(24);

  // Lower threshold velocity for switching on smart energy CoolStep and StallGuard to DIAG output
  driver_ptr_->TCOOLTHRS(0xFFFFF); // 20bit max
  
  // CoolStep lower threshold [0... 15].
  // If SG_RESULT goes below this threshold, CoolStep increases the current to both coils.
  // 0: disable CoolStep
  //driver_ptr_->semin(5);
  driver_ptr_->semin(0); // dissable -> interferences with stall guard

  // CoolStep upper threshold [0... 15].
  // If SG is sampled equal to or above this threshold enough times,
  // CoolStep decreases the current to both coils.
  driver_ptr_->semax(2);

  // Sets the number of StallGuard2 readings above the upper threshold necessary
  // for each current decrement of the motor current.
  driver_ptr_->sedn(0b01);

  // StallGuard4 threshold [0... 255] level for stall detection. It compensates for
  // motor specific characteristics and controls sensitivity. A higher value gives a higher
  // sensitivity. A higher value makes StallGuard4 more sensitive and requires less torque to
  // indicate a stall. The double of this value is compared to SG_RESULT.
  // The stall output becomes active if SG_RESULT fall below this value.
  driver_ptr_->SGTHRS(this->stall_threshold_);

  // Stand still option when motor current setting is zero (I_HOLD=0).
  // %00: Normal operation
  // %01: Freewheeling
  // %10: Coil shorted using LS drivers
  // %11: Coil shorted using HS drivers
  driver_ptr_->freewheel(this->freewheeling_mode_);
  if (this->freewheeling_mode_ > 0)
    driver_ptr_->ihold(0);

}



void TMC2209::dump_config() {
  ESP_LOGCONFIG(TAG, "TMC2209:");
  LOG_PIN("  Enable Pin: ", this->enable_pin_);
  LOG_PIN("  Index Pin: ", this->index_pin_);
  if (this->diag_pin_) LOG_PIN("  Diag Pin: ", this->diag_pin_);
  if (this->step_pin_) LOG_PIN("  Step Pin: ", this->step_pin_);
  if (this->dir_pin_) LOG_PIN("  Dir Pin: ", this->dir_pin_);
  ESP_LOGCONFIG(TAG, "  r_sense: %f",this->r_sense_);
  ESP_LOGCONFIG(TAG, "  freewheeling mode_: %X",this->freewheeling_mode_);
  ESP_LOGCONFIG(TAG, "  inverted: %s",this->inverted_ ? "True" : "False");
  ESP_LOGCONFIG(TAG, "  driver_address: %X",this->driver_address_);
  ESP_LOGCONFIG(TAG, "  stall_threshold: %d",this->stall_threshold_);
  ESP_LOGCONFIG(TAG, "  microsteps: %X",this->microsteps_);
  ESP_LOGCONFIG(TAG, "  rms_current: %f",this->rms_current_);
  ESP_LOGCONFIG(TAG, "  current position: %d",this->current_position);
  ESP_LOGCONFIG(TAG, "  target position: %d",this->target_position);
  ESP_LOGCONFIG(TAG, "  min_step: %d",this->min_step_);
  ESP_LOGCONFIG(TAG, "  max_step: %d",this->max_step_);

  LOG_STEPPER(this);
}



void IRAM_ATTR TMC2209::index_gpio_intr(TMC2209 *stepper) {
  // This is an interrupt handler - we can't call any virtual method from this method
  stepper -> index_counter_ ++;

}



void TMC2209::loop() {

#if USE_VACTUAL
  int32_t index_counter_local = index_counter_;
  index_counter_ = 0;

  if (this->get_dir()) {
    this->report_position(min(this->get_max_step(), this->current_position + (index_counter_local * STEPS_PER_INDEX)));
  } else {
    //stepper->current_position -= (index_counter_local * STEPS_PER_INDEX);
    this->report_position(max(this->get_min_step(), this->current_position - (index_counter_local * STEPS_PER_INDEX)));
  }
#endif

  bool at_target = this->has_reached_target();

  if (!at_target && (this -> is_stallguard_status == ENABLED) && this->diag_pin_->digital_read()) {
    ESP_LOGW(TAG, "Stalled with SG_RESULT=%d current_position=%d", this->driver_ptr_->SG_RESULT(), this->current_position);

    if (this -> is_calibrating_) {
      if (this->get_dir()) {
        //opening
        this->max_step_ = this->current_position;
         ESP_LOGD(TAG, "Calibrated max_step at %d.", this->max_step_);
        //start calibrating closing
        this->set_timeout(1000, [this]() {
          this->min_step_ = std::numeric_limits<int32_t>::min()/2;
          this->set_target(this->min_step_);
        });
      } else {
        //closing
        this->min_step_ = this->current_position;
         ESP_LOGD(TAG, "Calibrated min_step at %d.", this->min_step_);
        //End calibration
        ESP_LOGD(TAG, "Calibration ended...");
        this->is_calibrating_ = false;
      }
    }

    // Stop stepper
    if (this->get_dir())
      this->current_position = this->get_max_step();
    else
      this->current_position = this->get_min_step();
    this->target_position = this->current_position;
  }

  if (at_target) {
    this->high_freq_.stop();
    driver_ptr_->VACTUAL(0);
    this -> is_stallguard_status = DISSABLED;
    return;
  }

  this->high_freq_.start();

  // Enable stall guard after 100 ms moving
  if (this -> is_stallguard_status == DISSABLED) {
    this -> is_stallguard_status = ENABLING;
    this->set_timeout(500, [this]() {
        this -> is_stallguard_status = ENABLED;
    });
  }
#if USE_VACTUAL
  uint32_t now = micros();
  this->calculate_speed_(now);

  int32_t new_vactual = this->current_speed_ * MSTEP_T;
  if (this->get_dir() != this->inverted_) new_vactual = 0 - new_vactual;

  //if (abs(this->current_speed_ - actual_speed) >= 10) {
     driver_ptr_->VACTUAL(new_vactual);
  //}
#else
  int32_t dir = this->should_step_();
  if (dir == 0)
    return;  

  //stepping
  this->dir_pin_->digital_write((dir == 1) != this->inverted_); // != acts as logical xor
  this->step_pin_->digital_write(true);
  delayMicroseconds(5);
  this->step_pin_->digital_write(false);
#endif
}

void TMC2209::calibrate() {
  ESP_LOGD(TAG, "Calibrating...");
  this->is_calibrating_ = true;

  this->report_position(0);
  this->max_step_ = std::numeric_limits<int32_t>::max()/2;
  this->set_target(this->max_step_);


}

}  // namespace tmc2209
}  // namespace esphome