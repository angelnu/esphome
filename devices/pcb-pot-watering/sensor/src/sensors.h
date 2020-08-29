#include "Arduino.h"
#define DEFAULT_AVERAGE_MEASUREMENTS 5

void sensors_setup();

struct sensor_values_t {
    uint16_t soil_raw;
    uint16_t soil_p;
    uint16_t temp_dC;
};

sensor_values_t sensors_read();