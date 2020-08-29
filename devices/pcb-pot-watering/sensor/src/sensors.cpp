#include "sensors.h"
#include "config.h"
#include <CapacitiveSensor.h>

CapacitiveSensor* mySensor_ptr;

void sensors_setup() {
  analogReadResolution(ADC_RESOLUTION);

  //Soil sensor shield
  pinMode(PB0, OUTPUT);
  pinMode(PB1, OUTPUT);
  digitalWrite(PB0, LOW);
  digitalWrite(PB1, LOW);
  mySensor_ptr   = new CapacitiveSensor(PIN_CAPACITY_SEND, PIN_CAPACITY_READ);
}

#define ADC_RANGE (1<<ADC_RESOLUTION)
#if defined(STM32L0xx)
//See https://www.mouser.de/datasheet/2/389/stm32l010k8-1851374.pdf
#define ADC_VREFINT_3V0_30C_ADDR 0x1FF80078
#elif defined(STM32L4xx)
//See https://www.mouser.de/datasheet/2/389/stm32l412c8-1851177.pdf
#define ADC_VREFINT_3V0_30C_ADDR 0x1FFF75AA
#define ADC_TEMP_3V0_30C_ADDR  0x1FFF75A8 
#define ADC_TEMP_3V0_130C_ADDR 0x1FFF75CA
#else
#error Unknown STM32 MMU
#endif

// Average
uint32_t averageAnalogInput(uint32_t pin, uint8_t measurements) {
  uint32_t total = 0;
  for (uint8_t i=0; i<measurements; i++){
    total += analogRead(pin);
  }
  return total / measurements;

}

// VREF

//See https://github.com/stm32duino/Arduino_Core_STM32/pull/539
const uint16_t* const ADC_VREFINT_3V0_30C =  reinterpret_cast<uint16_t*>(ADC_VREFINT_3V0_30C_ADDR);
const uint32_t CALIBRATION_REFERENCE_VOLTAGE = 3000;
const float VREFINT = CALIBRATION_REFERENCE_VOLTAGE * (*ADC_VREFINT_3V0_30C) / ADC_RANGE;

uint32_t readVref()
{
  return (VREFINT * ADC_RANGE / averageAnalogInput(AVREF, device_config.measure_average)); // ADC sample to mV
}


// TEMPERATURE
#ifdef ADC_TEMP_3V0_30C_ADDR
    // see datasheet for position of the calibration values, this is for STM32L4xx
    const uint16_t* const ADC_TEMP_3V0_30C =  reinterpret_cast<uint16_t*>(ADC_TEMP_3V0_30C_ADDR);
    const uint16_t* const ADC_TEMP_3V0_130C =  reinterpret_cast<uint16_t*>(ADC_TEMP_3V0_130C_ADDR);


    float readTempSensor(int32_t VRef)
    {
    // scale constants to current reference voltage
    float adcCalTemp30C = static_cast<float>(*ADC_TEMP_3V0_30C);
    float adcCalTemp130C = static_cast<float>(*ADC_TEMP_3V0_130C);

    //Read and convert to calibration value
    float adcTempValue = 1.0 * averageAnalogInput(ATEMP, device_config.measure_average) * VRef / CALIBRATION_REFERENCE_VOLTAGE;

    return (adcTempValue - adcCalTemp30C)/(adcCalTemp130C - adcCalTemp30C) * (130.0 - 30.0) + 30.0;
    }
#else
  float readTempSensor(int32_t VRef) {return 0;}
#endif

sensor_values_t sensors_read () {

  sensor_values_t values;
  //Read sensors
  uint32_t VRef = readVref();
  float temp_c = readTempSensor(VRef);
  float soilRaw = 1.0 * mySensor_ptr->capacitiveSensorRaw(device_config.measure_average) / device_config.measure_average;
  float soil_p = 100.0 * (soilRaw - device_config.min_s) / device_config.max_s;
  if (soil_p > 100)
    soil_p = 100;

  //Debug output
  log_debug(F("SOIL %: "));
  log_debug_ln(soil_p);
  log_debug(F("TEMP: "));
  log_debug_ln(temp_c, 1);
  log_debug(F("SOIL: "));
  log_debug_ln(soilRaw);

  //Copy values
  values.temp_dC = temp_c * 10;
  values.soil_raw = soilRaw;
  values.soil_p = soil_p * 10;

  return values;
}