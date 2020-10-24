#include "colors.h"

void light_effect_sunset() {
    static int step_duration = 5*60*1000;  //in ms
    static int state = 0;
    auto call = id(my_light).turn_on(); //put the id for your light in here
    
    if (state == 0) 
    {
        call.set_rgbw(color_parms(color_sunday));
        //call.set_brightness(0.7);
        call.set_transition_length(1000);
        state ++;
    }
    else if (state == 1)
    {
        call.set_rgbw(color_parms(color_twinlight));
        call.set_brightness(1.0);
        call.set_transition_length(step_duration);
        state ++;
    }
    else if (state == 2)
    {
        call.set_rgbw(color_parms(color_night));
        call.set_brightness(0.5);
        call.set_transition_length(step_duration);
        state ++;
    }
    else{
        state = 0;
        call.set_effect("Night");
    }
    call.perform();
}