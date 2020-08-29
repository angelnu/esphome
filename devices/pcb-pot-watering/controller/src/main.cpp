#include "plant_sensor.h"
#include <SPI.h>
#include <arduino.h>

#define serialPortSpeed 115200
#define PIN_SPI_MISO 19
#define PIN_SPI_MOSI 23
#define PIN_SPI_SCK 18
#define PIN_SPI_NSS 5

#define log_flush Serial.flush
#define log_error Serial.print
#define log_error_ln Serial.println
#define log_warning Serial.print
#define log_warning_ln Serial.println
#define log_info Serial.print
#define log_info_ln Serial.println
#if DEBUG
  #define log_debug Serial.print
  #define log_debug_ln Serial.println
#else
  #define log_debug if (false) Serial.print
  #define log_debug_ln if (false) Serial.println
#endif


SPIClass spi;
Plant_sensor* plant_p;

void setup(){

  //initialise vspi with default pins
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  spi.begin(PIN_SPI_SCK ,PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_NSS);

  plant_p = new Plant_sensor(spi, PIN_SPI_NSS);
}




void print_hex_mem(std::vector<uint8_t>& data, bool bigEndian=true) {
    log_info   (F("0x"));
    for (size_t i=0; i < data.size(); i++) {
        size_t ii = bigEndian ? i : data.size()-(i+1);
        uint8_t byte = *(data.data() + ii);
        if (byte < 16)
            log_info('0');
        log_info (byte, HEX);
    }
    log_info_ln("");
}

void loop(){
  plant_p->refresh_all();

  log_info(F("Version: "));
  log_info(plant_p->get_version());

  log_info(F("UID: "));
  log_info(plant_p->get_uid().c_str());

  log_info(F("Temp: "));
  log_info(plant_p->get_temp_C());

  log_info(F("Soil Humidity %: "));
  log_info(plant_p->get_soil_p());

  log_info(F("Soil umidity raw: "));
  log_info(plant_p->get_soil_raw());

  log_info(F("Build Date: "));
  log_info(plant_p->get_build_date().c_str());

  log_info(F("Build Time: "));
  log_info(plant_p->get_build_time().c_str());

  log_info(F("Build Commit: "));
  log_info(plant_p->get_build_commit().c_str());

  plant_p->set_watering_step_s(5);

  delay(10*1000);

}
