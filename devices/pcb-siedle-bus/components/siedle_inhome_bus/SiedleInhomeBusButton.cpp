#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "SiedleInhomeBusButton.h"


namespace esphome {
namespace siedle_inhome_bus {

static const char *TAG = "siedle_inhome_bus.button";


void SiedleInhomeBusButton::dump_config(){

    LOG_BUTTON("", "Siedle In-Home Bus Button", this);
    LOG_SIEDLE_COMMAND("  ", command_);
}

void SiedleInhomeBusButton::press_action() {
    this->parent_->send_cmd(*this->command_);
}


}  // namespace siedle_inhome_bus
}  // namespace esphome