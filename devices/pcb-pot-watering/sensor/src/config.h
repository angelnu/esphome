#include "arduino.h"

#define DEVICE_CONFIG_VERSION 4
struct device_config_t {
  //Common part
  uint8_t version_config;
  uint8_t version;
  uint8_t is_debug;
  uint8_t reserved;
  uint8_t measure_average;
  //Soil sensor
  uint16_t min_s;
  uint16_t max_s;
};
extern device_config_t device_config;

#define DEFAULT_AVERAGE_MEASUREMENTS 5
#define DEFAULT_MIN_S 400
#define DEFAULT_MAX_S 1000
#define UID_BYTES_LENGTH 12

typedef uint32_t crc_t;
#define CRC_SIZE sizeof(crc_t)
#define EEPROM_BLOCK_SIZE 0x100
#define EEPROM_BLOCKS 8
union external_data_block_t {
  uint8_t block[EEPROM_BLOCK_SIZE];
  struct {
    uint8_t data[EEPROM_BLOCK_SIZE-sizeof(crc_t)];
    crc_t crc;
  } parts;
};
extern external_data_block_t external_data[EEPROM_BLOCKS];

void init_device_config(bool reset=false);
void write_device_config(device_config_t &device_config);
void print_buildinfo();
void read_external_data(size_t block);
void write_external_data(size_t block);
crc_t get_external_data_crc(size_t block);

#ifndef DEBUG
  #define DEBUG 0
#endif

//Serial port
#define serialPortSpeed 115200
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

#define Q(x) #x
#define QUOTE(x) Q(x)
#define GIT_COMMIT_ID QUOTE(PIO_SRC_REV)