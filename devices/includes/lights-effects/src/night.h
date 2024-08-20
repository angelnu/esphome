#include "colors.h"

void light_effect_night() {
    auto call = id(my_light).turn_on(); //put the id for your light in here
    call.set_rgbw(color_parms(color_night));
    // new_moon, waxing_crescent, first_quarter, waxing_gibbous, full_moon, waning_gibbous, last_quarter or waning_crescent
    std::string moon_phase = id(my_moon_sensor).state;
    //it.print("Moon Phase: %s", moon_phase.c_str());
    ESP_LOGI("main", "Value of my sensor Moon Phase: %s", moon_phase.c_str());
    if (moon_phase == "new_moon") 
    {
        call.set_brightness(0.1);
    }
    else if (moon_phase == "waxing_crescent" or moon_phase == "waning_crescent") 
    {
        call.set_brightness(0.2);
    }
    else if (moon_phase == "first_quarter" or moon_phase == "last_quarter") 
    {
        call.set_brightness(0.3);
    }
    else if (moon_phase == "waxing_gibbous" or moon_phase == "waning_gibbous") 
    {
        call.set_brightness(0.4);
    }
    else if (moon_phase == "full_moon") 
    {
        call.set_brightness(0.5);
    }

    call.set_transition_length(10000);
    call.perform();
}