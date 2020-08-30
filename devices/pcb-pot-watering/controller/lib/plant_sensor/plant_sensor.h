#ifndef PLANT_SENSOR_H
#define PLANT_SENSOR_H

#include "Arduino.h"
#include <SPI.h>
#include <vector>
#include <string>
#include <functional>

static const auto PLANT_NAME_MAX_LENGTH = 80;
static const auto PLANT_DESCRIPTION_MAX_LENGTH = 250;
static const auto PLANT_BLOCK_SIZE = 0x100;
static const auto TRANFER_RETRIES = 1;

class Plant_sensor {
    public:
        Plant_sensor(SPIClass& spi, int8_t ss);

        int refresh_sensors();
        int refresh_all();
        bool is_error() { return iv_is_error;}

        //Read-only
        const uint8_t      get_version()          { return     iv_block_0.version            ;}
        const bool         is_debug()             { return     iv_block_0.is_debug           ;}
        const std::string& get_uid()              { return     iv_uid                        ;}
        const std::string& get_build_date()       { return     iv_build_date                 ;}
        const std::string& get_build_time()       { return     iv_build_time                 ;}
        const std::string& get_build_commit()     { return     iv_build_commit               ;}
        
        const uint16_t     get_soil_raw()         { return     iv_sensor_values.soil_raw     ;}
        const float        get_soil_p()           { return 1.0*iv_sensor_values.soil_p  / 10 ;}
        const float        get_temp_C()           { return 1.0*iv_sensor_values.temp_dC / 10 ;}
        
        //Read-write
        uint8_t    get_measure_average ()         { return     iv_block_0.measure_average    ;}
        int        set_measure_average            ( const uint8_t measure_average) {
            iv_block_0.measure_average=measure_average;
            return send_block_0();
        }
        uint16_t   get_min_s()                    { return   iv_block_0.min_s                ;}
        int        set_min_s                      ( uint16_t min_s ) {
            iv_block_0.min_s=min_s;
            return send_block_0();
        }
        uint16_t   get_max_s ()                   { return iv_block_0.max_s                  ;}
        int        set_max_s                      ( uint16_t max_s ) {
            iv_block_0.max_s=max_s;
            return send_block_0();
        }
        uint8_t    get_min_humidity_percentage()  { return iv_block_1.min_humidity_percentage;}
        int        set_min_humidity_percentage    ( uint8_t min_humidity_percentage) {
            iv_block_1.min_humidity_percentage=min_humidity_percentage;
            return send_block_1();
        }
        uint8_t     get_max_humidity_percentage() { return iv_block_1.max_humidity_percentage;}
        int         set_max_humidity_percentage   ( uint8_t max_humidity_percentage) {
            iv_block_1.max_humidity_percentage=max_humidity_percentage;
            return send_block_1();
        }
        uint8_t     get_watering_step_s()         { return iv_block_1.watering_step_s        ;}
        int         set_watering_step_s           ( uint8_t watering_step_s) {
            iv_block_1.watering_step_s=watering_step_s;
            return send_block_1();
        }
        std::string get_name()                    { return iv_block_1.name                   ;}
        int         set_name                      ( std::string name ) {
            strncpy(iv_block_1.name, name.c_str(), sizeof(iv_block_1.name));
            return send_block_1();
        }

    private:
        SPIClass& iv_spi;
        int8_t iv_ss;

        bool iv_is_error;

        std::string iv_uid;
        std::string iv_build_date;
        std::string iv_build_time;
        std::string iv_build_commit;

        struct sensor_values_t {
            uint16_t soil_raw;
            uint16_t soil_p;
            uint16_t temp_dC;
        } iv_sensor_values;

        void begin_transaction();
        void end_transaction();

        enum command_t {
            COMMAND_GET_DEVICE_VERSION=11,
            COMMAND_REFRESH_SENSOR_DATA=12,
            COMMAND_GET_DEVICE_UID=13,
            COMMAND_GET_BUILD_DATE=14,
            COMMAND_GET_BUILD_TIME=15,
            COMMAND_GET_GIT_COMMIT_ID=16,
            COMMAND_GET_SENSOR_DATA=17,
            COMMAND_TEST=18,
            COMMAND_GET_BLOCK_CRC=19
        };
        enum command_block_t {
            COMMAND_GET_BLOCK=20,
            COMMAND_SEND_BLOCK=21,
        };
        typedef std::vector<uint8_t> command_out_t;
        command_out_t read_command(command_t command, uint8_t extension=0, size_t retriesLeft = TRANFER_RETRIES);

        int send_block(uint8_t block_index, uint8_t* data, uint8_t size, size_t retriesLeft = TRANFER_RETRIES);
        template<class T> int send_block(uint8_t block_index, T& block) {
            return send_block(block_index, (uint8_t*) &block, sizeof(block));
        }

        typedef std::function<int (uint8_t*)> init_block_t;
        int _receive_block(uint8_t block_index, uint8_t* data, uint8_t size, size_t version, init_block_t init_block, size_t retriesLeft = TRANFER_RETRIES);
        template<class T> int receive_block(uint8_t block_index, T& block, size_t version, init_block_t init_block) {
            return _receive_block(block_index, (uint8_t*) &block, sizeof(block), version, init_block);
        }
        
        static const auto BLOCK_0_VERSION = 4;
        struct block_0_t {
            //Common part
            uint8_t version_config;
            uint8_t version;
            uint8_t is_debug;
            uint8_t reserved;
            uint8_t measure_average;
            //Soil sensor
            uint16_t min_s;
            uint16_t max_s;
        } iv_block_0 = {0};
        int init_block_0(uint8_t* old_data_p);
        int receive_block_0() {
            return receive_block(0, iv_block_0, BLOCK_0_VERSION, std::bind(&Plant_sensor::init_block_0, this, std::placeholders::_1));
        }int send_block_0() {
            return send_block(0, iv_block_0);
        }


        static const auto BLOCK_1_VERSION = 1;
        static const auto BLOCK_1_DEFAULT_MIN_HUMIDITY_PERCENTAGE = 5;
        static const auto BLOCK_1_DEFAULT_MAX_HUMIDITY_PERCENTAGE = 10;
        static const auto BLOCK_1_DEFAULT_WATERING_STEP_S = 15;
        struct block_1_t {
            uint8_t version;
            uint8_t min_humidity_percentage;
            uint8_t max_humidity_percentage;
            uint8_t watering_step_s;
            uint8_t reserved [60];
            char name [PLANT_NAME_MAX_LENGTH];
        } iv_block_1 = {0};
        int init_block_1(uint8_t* old_data_p);
        int receive_block_1() {
            return receive_block(1, iv_block_1, BLOCK_1_VERSION, std::bind(&Plant_sensor::init_block_1, this, std::placeholders::_1));
        }
        int send_block_1() {
            return send_block(1, iv_block_1);
        }

        
        static const auto BLOCK_2_VERSION = 1;
        struct block_2_t {
            uint8_t version;
            char description [PLANT_DESCRIPTION_MAX_LENGTH];
        } iv_block_2 = {0};
        int init_block_2(uint8_t* old_data_p);
        int receive_block_2() {
            return receive_block(2, iv_block_2, BLOCK_2_VERSION, std::bind(&Plant_sensor::init_block_2, this, std::placeholders::_1));
        }
        int send_block_2() {
            return send_block(2, iv_block_2);
        };

};

#endif //PLANT_SENSOR_H