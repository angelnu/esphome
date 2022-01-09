#include "i2c_platform.h"
#include <vector>

std::vector<esphome::i2c::I2CDevice*> &geHandlesVector()
{
  static std::vector<esphome::i2c::I2CDevice*> s_vector;
  return s_vector;
}

//Register an i2c component and get a handle for i2c_platform functions
uint8_t i2c_platform_register_component(esphome::i2c::I2CDevice* devicePtr){
  geHandlesVector().push_back(devicePtr);
  return geHandlesVector().size() - 1;
}

//Write to i2c register
int32_t i2c_platform_write(uint16_t handle, uint8_t *bufp, size_t len) {
    return geHandlesVector()[handle]->write(bufp, len);
}

//Read from i2c register
int32_t i2c_platform_read(uint16_t handle, uint8_t *bufp, size_t len) {
    return geHandlesVector()[handle]->read(bufp, len);
}

//delay mseconds
void i2c_platform_delay(uint16_t handle, uint16_t wait_ms) { 
  delay(wait_ms);
}