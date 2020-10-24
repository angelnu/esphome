#include "colors.h"

void light_effect_sunrise() {
    
      static int duration = 5*60*1000;  //in ms
      static int state = 0;
      auto call = id(my_light).turn_on(); //put the id for your light in here
      
      if (state == 0) 
      {
        call.set_rgbw(color_parms(color_night));
        call.set_brightness(0.5);
        call.set_transition_length(10000);
      }
      else if (state == 1)
      {
          call.set_rgbw(color_parms(color_sunday));
          call.set_brightness(1.0);
          call.set_transition_length(duration); // in ms
          
      }
      else if (state == 2){
          state = 0;
          call.set_effect("Sunday");
      }
      state ++;
      call.perform();
}