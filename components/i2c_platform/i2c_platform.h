#pragma once

#include "esphome.h"
#include <stdint.h>

extern "C" {
    //Register an i2c component and get a handle for i2c_platform functions
    uint8_t i2c_platform_register_component(esphome::i2c::I2CDevice* devicePtr);

    //Write to i2c register
    int32_t i2c_platform_write(uint16_t handle, uint8_t *bufp, size_t len);

    //Read from i2c register
    int32_t i2c_platform_read(uint16_t handle, uint8_t *bufp, size_t len);

    //delay mseconds
    void i2c_platform_delay(uint16_t handle, uint16_t wait_ms);
}