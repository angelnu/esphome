#include "colors.h"

void light_effect_rgb_breathing() {
    static const color_rgbw_t colors[] = {
        {1.0, 0.0, 0.0 ,1.0},
        {1.0, 0.5, 0.0 ,1.0},
        {1.0, 1.0, 0.0 ,1.0},
        {0.5, 1.0, 0.0 ,1.0},
        {0.0, 1.0, 0.0 ,1.0},
        {0.0, 1.0, 0.5 ,1.0},
        {0.0, 1.0, 1.0 ,1.0},
        {0.0, 0.5, 1.0 ,1.0},
        {0.0, 0.0, 1.0 ,1.0},
        {0.5, 0.0, 1.0 ,1.0},
        {0.5, 0.0, 1.0 ,1.0},
        {1.0, 0.0, 0.5 ,1.0},
    };

    static int state = 0;
    static int color = 0;
    auto call = id(my_light).turn_on(); //put the id for your light in here
    call.set_transition_length(15000);
    if (state == 0) 
    {
        call.set_brightness(0.01);   
    }
    else if (state == 1)
    {
        call.set_rgbw(color_parms(colors[color]));
        call.set_brightness(1.0);
    }
        
    state ++;
    if (state == 2){
        state = 0;
    }
    color++;
    if(color >= sizeof(colors))
    {
        color = 0;
    }
    call.perform();
}