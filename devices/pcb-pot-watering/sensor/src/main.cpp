#include "config.h"
#include "sensors.h"
#include "spi_slave.h"
#include "IWatchdog.h"
#include "CRC32.h"

SPI_Slave spi;
union{
  uint8_t bulk[2];
  struct {
    uint8_t main;
    uint8_t extension;
  };
} command={};

static void send_command_response(uint8_t* data, uint8_t size);
static void command_received();
static void received_config();
static void end_command() {
  log_info(F("<< End command: "));
  log_info_ln(command.main);
  command = {};
  spi.end();
}

static void nss_interrupt_handler(){

  if (digitalRead(PIN_SPI_NSS) == HIGH){

    if (command.main != 0) {
      log_warning(F("abort command : "));
      log_warning_ln(command.main);
    
      spi.abort();
      command = {};
    }
    spi.end();
  } else {
    spi.begin();
    spi.transfer_IT(sizeof(command), 0, 0, command.bulk, command_received);
  }

}

sensor_values_t sensor_values;

void setup() {
    #ifdef DEBUG_DURING_SLEEP
      HAL_DBGMCU_EnableDBGStopMode();
    #endif

    Serial.begin(serialPortSpeed);
    
    #ifdef IWDG_TIME_S
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        __HAL_RCC_CLEAR_RESET_FLAGS();
        log_info_ln(F("Reset from watchdog"));
      #ifdef PIN_STATUS_LED
        //Blink error
        pinMode(PIN_STATUS_LED, OUTPUT);
        for (size_t i=0;i<5;i++) {
          digitalWrite(PIN_STATUS_LED, HIGH);
          delay(200);
          digitalWrite(PIN_STATUS_LED, LOW);
          delay(200);
        }
      #endif
        log_flush();
      }

      //Enable watchdog
      IWatchdog.begin(IWDG_TIME_S*1000*1000);
      
    #endif

    //Give a few seconds to connect programmer
    #ifdef PIN_STATUS_LED
      pinMode(PIN_STATUS_LED, OUTPUT);
      digitalWrite(PIN_STATUS_LED, HIGH);
    #endif
  
    log_flush();

    delay(5*1000);
    init_device_config();
    print_buildinfo();
    log_info_ln(F("Starting"));
    sensors_setup();
    spi.setMISO(PIN_SPI_MISO);
    spi.setMOSI(PIN_SPI_MOSI);
    spi.setSCLK(PIN_SPI_SCK);
    spi.setSSEL(PIN_SPI_NSS);
    HAL_MspInit();
    //spi.begin();
    
    #ifdef PIN_STATUS_LED
      pinMode(PIN_STATUS_LED, OUTPUT);
      digitalWrite(PIN_STATUS_LED, LOW);
    #endif

    //Get initial data
    sensor_values = sensors_read();
    for (size_t block=0; block < EEPROM_BLOCKS; block++) {
      read_external_data(block);
    }

    attachInterrupt(digitalPinToInterrupt(PIN_SPI_NSS), nss_interrupt_handler, CHANGE);
    log_debug_ln(digitalRead(PIN_SPI_NSS));
}

bool need_sensor_data=false;
bool need_write_external_data[EEPROM_BLOCKS]={};
void loop(){
  #ifdef IWDG_TIME_S
      IWatchdog.reload();
  #endif

  if (need_sensor_data) {
    sensor_values = sensors_read();
    need_sensor_data = false;
  }

  for (size_t block=0; block < EEPROM_BLOCKS; block++) { 
    if (need_write_external_data[block]) {
      write_external_data(block);
      need_write_external_data[block] = false;
    }
  }
}

void command_received()
{
    log_debug(F(">> start command: "));
    log_debug(command.main);
    switch (command.main)
    {
    case 11:
      log_debug_ln(F(" - Query DEVICE VERSION"));
      send_command_response(&device_config.version, 1);
      break;
    case 12:
      log_debug_ln(F(" - Refresh sensor data"));
      need_sensor_data = true;
      send_command_response(0, 0);
      break;
    case 13:
      log_debug_ln(F(" - Query DEVICE UID"));
      send_command_response((uint8_t*)UID_BASE, UID_BYTES_LENGTH);
      break;
    case 14:
      log_debug_ln(F(" - Query BUILD DATE"));
      send_command_response((uint8_t*)__DATE__, sizeof(__DATE__));
      break;
    case 15:
      log_debug_ln(F(" - Query BUILD TIME"));
      send_command_response((uint8_t*)__TIME__, sizeof(__TIME__));
      break;
    case 16:
      log_debug_ln(F(" - Query GIT COMMIT ID"));
      send_command_response((uint8_t*)GIT_COMMIT_ID, sizeof(GIT_COMMIT_ID));
      break;
    case 17: {
      log_debug_ln(F(" - Get values"));
      send_command_response((uint8_t*)&sensor_values, sizeof(sensor_values));
      break;
    }
    case 18: {
      log_debug_ln(F(" - Test"));
      uint8_t test_output [5];
      test_output[0] = 1;
      test_output[1] = 2;
      test_output[2] = 3;
      test_output[3] = 4;
      test_output[4] = 5;
      send_command_response(test_output, sizeof(test_output));
      break;
    }
    case 19: {
      log_debug_ln(F(" - Get external data crc for block "));
      log_debug_ln(command.extension);
      crc_t crc = get_external_data_crc(command.extension);
      send_command_response((uint8_t*)&crc, sizeof(crc));
      break;
    }
    case 20: {
      log_debug(F(" - Read data for block "));
      log_debug_ln(command.extension);
      spi.transfer_IT(sizeof(external_data_block_t), external_data[command.extension].block, end_command);
      break;
    }
    case 21: {
      if (need_write_external_data[command.extension]) {
        log_debug(F("Still writting conging - skip"));
        end_command();
      }
      log_debug(F(" - Write config for block: "));
      log_debug_ln(command.extension);
      spi.transfer_IT(sizeof(external_data_block_t), 0,0, external_data[command.extension].block, received_config);
      break;
    }
    default:
      log_error(F(" - Unknown command: "));
      log_error_ln(command.main);
      end_command();
      break;
    }
}

void send_command_response(uint8_t* data, uint8_t size) {
  static uint8_t tx_buffer[EEPROM_BLOCK_SIZE];
  //Length - 1 byte
  tx_buffer[0] = size;
  //Payload
  memcpy(&tx_buffer[1], data, size);
  //CRC32 - 4 bytes
  CRC32 crcTool;
  crcTool.reset();
  crc_t crc = crcTool.calculate(data, size);
  memcpy(&tx_buffer[1 + size], &crc, sizeof(crc_t));
  //Send
  spi.transfer_IT(1 + size + CRC_SIZE, tx_buffer, end_command);
}

void received_config() {
  log_debug_ln(F("Received config"));
  need_write_external_data[command.extension]=true;
  end_command();
}