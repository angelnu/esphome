#include "colors.h"

void light_effect_sunday() {
    auto call = id(my_light).turn_on(); //put the id for your light in here
    call.set_rgbw(color_parms(color_sunday));
    call.set_brightness(1.0);
    call.set_transition_length(10000);
    call.perform();
}