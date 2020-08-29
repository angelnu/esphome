#include "plant_sensor.h"
#include "CRC32.h"

static const auto DELAY_SPI_MS=100;
static const int spiClk = 1000000; // 1 MHz
SPISettings spi_settings = SPISettings(spiClk, MSBFIRST, SPI_MODE0);

//Serial port
#define DEBUG 1
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


Plant_sensor::Plant_sensor(SPIClass& spi, int8_t ss, std::string default_name):
    iv_spi(spi),
    iv_ss(ss),
    iv_default_name(default_name) {

    pinMode(iv_ss, OUTPUT); //VSPI SS

    //Force reset
    begin_transaction();
    end_transaction();

    refresh_all();
}

int Plant_sensor::refresh_all() {

    bool is_error = false;
    //Get UID
    auto command_out = read_command(COMMAND_GET_DEVICE_UID);
    if (command_out.size() == 0) {
        log_error_ln(F("Cannot fetch UID"));
        is_error = true;
    } else {
        char uid_c[2*command_out.size()+1] = {0};;
        for (size_t i=0; i<command_out.size(); i++) {
            sprintf(uid_c+(2*i), "%02x", command_out[i]);
        }
        iv_uid = uid_c;
    }
    
    //Get build date
    command_out = read_command(COMMAND_GET_BUILD_DATE);
    if (command_out.size() == 0) {
        log_error_ln(F("Cannot fetch build date"));
        is_error = true;
    } else {
        iv_build_date = (char*)command_out.data();
    }

    //Get build time
    command_out = read_command(COMMAND_GET_BUILD_TIME);
    if (command_out.size() == 0) {
        log_error_ln(F("Cannot fetch build time"));
        is_error = true;
    } else {
        iv_build_time= (char*)command_out.data();
    }

    //Get build commit ID
    command_out = read_command(COMMAND_GET_GIT_COMMIT_ID);
    if (command_out.size() == 0) {
        log_error_ln(F("Cannot fetch build commit id"));
        is_error = true;
    } else {
        iv_build_commit = (char*)command_out.data();
    }

    //Get Blocks
    if (receive_block_0() !=0) is_error = true;
    if (receive_block_1() !=0) is_error = true;
    //receive_block_2();

    //Get sensors
    if (refresh_sensors() !=0) is_error = true;

    //Remember last
    iv_is_error = is_error;
    return is_error;
}

int Plant_sensor::refresh_sensors() {
    bool is_error = false;
    
    auto command_out = read_command(COMMAND_REFRESH_SENSOR_DATA);
    delay(50);
    command_out = read_command(COMMAND_GET_SENSOR_DATA);
    if (command_out.size() == 0) {
        log_error_ln(F("Cannot fetch sensor data"));
        is_error = true;
    } else {
        iv_sensor_values = *((sensor_values_t*)command_out.data());
    }
    return is_error;
}

int Plant_sensor::init_block_0(uint8_t* old_data_p) {
    log_error_ln(F("Cannot init block 0"));
    return -1;
}

int Plant_sensor::init_block_1(uint8_t* old_data_p) {

    if (old_data_p) {
        log_error_ln(F("Upgrade block 1 not implemented"));
        return -1;
    }
    iv_block_1.version = BLOCK_1_VERSION;
    iv_block_1.min_humidity_percentage = BLOCK_1_DEFAULT_MIN_HUMIDITY_PERCENTAGE;
    iv_block_1.max_humidity_percentage = BLOCK_1_DEFAULT_MAX_HUMIDITY_PERCENTAGE;
    iv_block_1.watering_step_s = BLOCK_1_DEFAULT_WATERING_STEP_S;
    strncpy(iv_block_1.name, (std::string("New plant")+std::string(iv_default_name)).c_str(), sizeof(iv_block_1.name));
    return send_block_1();

}

int Plant_sensor::init_block_2(uint8_t* old_data_p) {
    return send_block_2();
}

void Plant_sensor::begin_transaction() {
    digitalWrite(iv_ss,LOW);
    delay(DELAY_SPI_MS);
    iv_spi.beginTransaction(spi_settings); 
}

void Plant_sensor::end_transaction() {
    iv_spi.endTransaction();
    delay(DELAY_SPI_MS);
    digitalWrite(iv_ss,HIGH);
    delay(DELAY_SPI_MS);
}

Plant_sensor::command_out_t Plant_sensor::read_command(Plant_sensor::command_t command, uint8_t extension, size_t retriesLeft) {
 
    //Print command
    log_debug(F("COMMAND: "));
    log_debug_ln(command);
        
    //Begin
    begin_transaction();

    //Send command
    uint16_t commandIn  = 0x100 * iv_spi.transfer(command);
            commandIn +=         iv_spi.transfer(extension);
    
    //Check command to ensure we are ready
    if (commandIn) {
        //Error
        log_error(F("UNEXPECTED VALUE SEND COMMAND : "));
        log_error_ln(commandIn);

        //End transaction
        end_transaction();

    } else {
    
        //OK

        //Wait for command reply
        delay(DELAY_SPI_MS);

        //Get result length
        uint8_t command_out_length=iv_spi.transfer((uint8_t)0);

        //Get data
        command_out_t command_out = command_out_t(command_out_length);
        command_out.reserve(command_out_length+4); //This is because transferBytes reads uint32_t so we need to avoid corrupting the heap
        iv_spi.transferBytes(NULL, command_out.data(), command_out.size());

        //Calculate CRC
        uint32_t calculated_crc = CRC32().calculate(command_out.data(), command_out.size());

        //Get CRC
        uint32_t crc;
        iv_spi.transferBytes(NULL, (uint8_t*)&crc, sizeof(crc));

        //End transaction
        end_transaction();

        //Check crc error
        if (calculated_crc == crc) {
            //print result
            log_debug(F("OUT LENGTH: "));
            log_debug_ln(command_out.size());

            return command_out;
        } else  {
            log_error_ln(F("read_command CRC MISTMATCH"));
        }
    }

    //Retries
    if (retriesLeft--) {
        log_warning_ln("Retry transaction");
        return read_command(command, retriesLeft);
    } else {
        log_error_ln("Aborting: no more reties");
        return command_out_t(0);
    }
  
}

int Plant_sensor::_receive_block(uint8_t block_index, uint8_t* data, uint8_t size, size_t version, init_block_t init_block, size_t retriesLeft){
    
    //Print block id
    log_debug(F("Receive Block: "));
    log_debug_ln(block_index);
    
    //Begin
    begin_transaction();
  
    //Send command
    uint16_t commandIn  = 0x100 * iv_spi.transfer(COMMAND_GET_BLOCK);
            commandIn +=         iv_spi.transfer(block_index);

    //Check command to ensure we are ready
    if (commandIn) {
        //Error
        log_error(F("UNEXPECTED VALUE SEND COMMAND : "));
        log_error_ln(commandIn);
        
        //End transaction
        end_transaction();

    } else {
    
        //OK

        //Wait for command reply
        delay(DELAY_SPI_MS);

        //Get block
        uint8_t rx_block[PLANT_BLOCK_SIZE];
        iv_spi.transferBytes(NULL, rx_block, sizeof(rx_block));

        //End
        end_transaction();

        //Calculate CRC
        uint32_t received_crc = *( (uint32_t*) &rx_block[sizeof(rx_block) - sizeof(received_crc)]);
        uint32_t calculated_crc = CRC32().calculate(rx_block, sizeof(rx_block) - sizeof(calculated_crc));

        

        //Check crc error
        if (received_crc != calculated_crc) {
            //CRC missmatch
            if (received_crc == 0) {
                //Empty
                log_info_ln(F("Empty block - init"));
                return init_block(0);
            } if (retriesLeft==0) {
                //Corrupted
                log_info_ln(F("corrupted block - init"));
                return init_block(0);
            } else {
                //Error
                log_error_ln(F("Block receive - CRC MISSMATCH"));
            }
        } else {
            if (version != rx_block[0]) {

                //different version
                log_warning(F("Upgrade from version "));
                log_warning_ln(rx_block[0]);
                return init_block(rx_block);

            } else{

                //OK - copy block
                memcpy(data, rx_block, size);

                //print result
                log_debug_ln(F("Block received sucesfully "));

                return 0;
            }
        }
    }

    //Retries
    if (retriesLeft--) {
        log_warning_ln("Retry transaction");
        return _receive_block(block_index, data, size, version, init_block, retriesLeft);
    } else {
        log_error_ln("Aborting: no more retries");
        return -1;
    }
}

int Plant_sensor::send_block(uint8_t block_index, uint8_t* data, uint8_t size, size_t retriesLeft) {
  
    //Print block id
    log_debug(F("Send Block: "));
    log_debug_ln(block_index);
    
    //Begin
    begin_transaction();
    
    //Send command
    uint16_t commandIn  = 0x100 * iv_spi.transfer(COMMAND_SEND_BLOCK);
            commandIn +=         iv_spi.transfer(block_index);

    //Check command to ensure we are ready
    if (commandIn) {
        //Error
        log_error(F("Block send - UNEXPECTED VALUE SEND COMMAND : "));
        log_error_ln(commandIn);
        
        //End transaction
        end_transaction();
    } else {
    
        //OK

        //Wait for command reply
        delay(DELAY_SPI_MS);


        //Create full block
        uint8_t external_data[0x100] = {};
        memcpy(external_data, data, size);

        //Add CRC
        uint32_t crc = CRC32().calculate(external_data, sizeof(external_data) - sizeof(crc)) ;
        *( (uint32_t*) &external_data[sizeof(external_data) - sizeof(crc)]) = crc;

        //Send block
        iv_spi.transferBytes(external_data, NULL, sizeof(external_data));

        //End
        end_transaction();

        //Retrieve block CRC
        uint32_t received_crc = *((uint32_t*)read_command(COMMAND_GET_BLOCK_CRC, block_index).data());

        //Check crc error
        if (received_crc == crc) {

            //print result
            log_debug_ln(F("Block sent sucesfully "));

            //OK
            return 0;
        } else  {
            //Error
            log_error_ln(F("Block send - CRC MISTMATCH"));
        }
    }

    //Retries
    if (retriesLeft--) {
        log_warning_ln("Retry transaction");
        return send_block(block_index, data, size, retriesLeft);
    } else {
        log_error_ln("Aborting: no more retries");
        return -1;
    }
}