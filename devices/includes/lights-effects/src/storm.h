#include "colors.h"

void light_effect_storm() {
    static int No_lighting = 10;
    static int state = 0;
    auto call = id(my_light).turn_on(); //put the id for your light in here
    call.set_rgbw(color_parms(color_skyblue));
    call.set_brightness(0.5);
    call.perform();
    
    if (rand() % 100 >= No_lighting)
    {
        call.set_rgbw(color_parms(color_lighting));
        call.set_brightness(1.0);
        call.set_flash_length(4); // in ms
        call.perform();
    }
}