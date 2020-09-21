#pragma once

#include "esphome.h"
namespace esphome {

namespace MC33996 {

class ICComponent;

class ICSwitch: public Switch {
    public:
        void set_switch_index(uint8_t index) {
            iv_index = index;
        }
        
        void set_parent_component(ICComponent& parent) {
            iv_parent_ptr = &parent;
        }

        void write_state(bool state) override ;

    private:
        uint8_t iv_index;
        ICComponent* iv_parent_ptr;
};

static const char *TAG = "MC33996";
class ICComponent : public esphome::Component {
    public:
        static const auto MAX_SWITCHES = 16;
        static const int spiClk = 1000000; // 1 MHz
        ICComponent(SPIClass& spi, uint8_t CS_pin, uint8_t reset_pin=0, uint8_t number_of_switches = MAX_SWITCHES):
            iv_spi(spi),
            iv_cs_pin(CS_pin),
            iv_spi_settings(spiClk, MSBFIRST, SPI_MODE1),
            iv_number_of_switches(number_of_switches)
        {
            for (size_t i=0; i<iv_number_of_switches; i++) {
                switches[i].set_switch_index(i);
                switches[i].set_parent_component(*this);
            }
        }

        void setup() override {
            ESP_LOGCONFIG(TAG, "Setting up MC33996...");

            // This will be called by App.setup()

            pinMode(iv_cs_pin,OUTPUT);
            digitalWrite(iv_cs_pin, HIGH);

            set_cmd(CMD_RESET, CMD_NULL);
            ESP_LOGCONFIG(TAG, "Setting up MC33996 done");
        }

        void dump_config() override {
            ESP_LOGCONFIG(TAG, "MC33996:");
            if (this->is_failed()) {
                ESP_LOGE(TAG, "Communication with MC33996 failed!");
                this -> setup();
            }
            //LOG_SWITCH(" ", "switch 0", &get_switch(0));
        }

        Switch& get_switch(uint8_t index) { return switches[index];}

        void write_state(uint8_t index, uint8_t state) {
            assert(index < iv_number_of_switches);
            if (state) 
                iv_chipPins |=  (1 << index);
            else
                iv_chipPins &= ~(1 << index);
            set_cmd(CMD_ON_OFF, iv_chipPins);
        }
    private:
        static const auto DELAY_SPI_MS = 5;
        SPIClass& iv_spi;
        uint8_t iv_cs_pin;
        SPISettings iv_spi_settings;
        uint8_t iv_number_of_switches;
        ICSwitch switches[MAX_SWITCHES];
        uint16_t iv_chipPins = 0;

        enum {
            CMD_ON_OFF = 0b00000000, //ON , OFF Command
            CMD_RESET  = 0b00011000, //Rest outputs to off
            CMD_SO_ANY = 0b10000000, //Query any error
            CMD_SO_OV  = 0b01000000, //Query overvoltage error
            CMD_NULL   = 0b0000000000000000, //Null command for last 16bits
        };
        uint16_t set_cmd(uint8_t reg, uint16_t value) {
            digitalWrite(iv_cs_pin,LOW);
            delay(DELAY_SPI_MS);
            iv_spi.begin();
            iv_spi.beginTransaction(iv_spi_settings);

            //iv_spi.setClockDivider(1000000); iv_spi.setDataMode(SPI_MODE1); iv_spi.setBitOrder(MSBFIRST);

            iv_spi.transfer(reg);
            uint16_t out = iv_spi.transfer16(value);

            iv_spi.endTransaction();
            delay(DELAY_SPI_MS);
            digitalWrite(iv_cs_pin, HIGH);
            delay(DELAY_SPI_MS);

            return out;
        }
};


void ICSwitch::write_state(bool state) {
    // This will be called every time the user requests a state change.

    iv_parent_ptr->write_state(iv_index, state);

    // Acknowledge new state by publishing it
    publish_state(state);
}

}
}