#pragma once

#include "esphome.h"
namespace esphome {

namespace LPS33HW {


#include "lps33hw_reg.h"

class PressionComponent: public PollingComponent, public i2c::I2CDevice {
    public:
        
        PressionComponent() : PollingComponent(2000) {};
        void setup() override;
        void dump_config() override;
        float get_setup_priority() const override;
        void update() override;
        void soft_reset();
        void sleep();
        void wake_up();
 
    protected:
        sensor::Sensor *temperature_sensor = new Sensor();
        sensor::Sensor *pression_sensor= new Sensor();
    
    private:
        stmdev_ctx_t dev_ctx;


};




static const char *TAG = "LPS33HWTR";
static const auto LPS33HWTR_I2C_ADD = 0x5D;

static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len){
    PressionComponent* parent = (PressionComponent*)(handle);
    return !parent->write_bytes(reg, bufp, len);
}
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len){
    PressionComponent* parent = (PressionComponent*)handle;
    return !parent->read_bytes(reg, bufp, len);
}
 
void PressionComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up LPS33HWTR...");

    //this->set_i2c_address(LPS33HWTR_I2C_ADD);
    /*
    *  Initialize mems driver interface
    */
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = (void*)this; 
    
    this->wake_up();
    this->soft_reset();

    
    ESP_LOGCONFIG(TAG, "Setting up LPS33HWTR done");
}

void PressionComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "LPS33HWTR:");
    //ESP_LOGCONFIG(TAG, " Model: %s", to_string(this->type_));
    LOG_I2C_DEVICE(this);
    if (this->is_failed()) {
        ESP_LOGE(TAG, "Communication with LPS33HWTR failed!");
        this -> setup();
    }
    LOG_UPDATE_INTERVAL(this);
  
    LOG_SENSOR(" ", "Temperature", this->temperature_sensor);
    LOG_SENSOR(" ", "Pression", this->pression_sensor);
 }

float PressionComponent::get_setup_priority() const { return setup_priority::DATA; }
void PressionComponent::update() {
    if (this->status_has_warning()) {
        ESP_LOGW(TAG, "Retrying to reconnect the sensor.");
        this->soft_reset();
    }

    this->wake_up();
 
    this->set_timeout(50, [this]() {
        /*
        * Read output only if new value is available
        */
        lps33hw_reg_t reg;
        if (lps33hw_read_reg(&dev_ctx, LPS33HW_STATUS, (uint8_t *)&reg, 1)) {
            ESP_LOGE(TAG, "Could not read status");
            this->status_set_warning();
            return;
        }

        if (reg.status.p_da)
        {
            typedef union{
                int32_t i32bit;
                uint8_t u8bit[4];
            } axis1bit32_t;
            axis1bit32_t data_raw_pressure;
            memset(data_raw_pressure.u8bit, 0x00, sizeof(int32_t));
            lps33hw_pressure_raw_get(&dev_ctx, data_raw_pressure.u8bit);
            float pressure_hPa = lps33hw_from_lsb_to_hpa( data_raw_pressure.i32bit);
            
            ESP_LOGV(TAG, "Got pression from %#04x: %.2f hPA", address_, pressure_hPa);
            this->pression_sensor->publish_state(pressure_hPa);
        } else {
            ESP_LOGW(TAG, "Could not read presion: %#04x", reg.byte);
        }

        if (reg.status.t_da)
        {
            typedef union{
                int16_t i16bit;
                uint8_t u8bit[2];
            } axis1bit16_t;
            axis1bit16_t data_raw_temperature;
            memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
            lps33hw_temperature_raw_get(&dev_ctx, data_raw_temperature.u8bit);
            float temperature_degC = lps33hw_from_lsb_to_degc( data_raw_temperature.i16bit );
            
            ESP_LOGV(TAG, "Got temperature from %#04x: %.2f °C", address_, temperature_degC);
            this->temperature_sensor->publish_state(temperature_degC);
        } else {
            ESP_LOGW(TAG, "Could not read temperature");
        }

        this->status_clear_warning();
        this->sleep();
    });
}

void PressionComponent::soft_reset() {
    /*
    *  Check device ID
    */
    uint8_t whoamI = 0;
    lps33hw_device_id_get(&dev_ctx, &whoamI);
    if ( whoamI != LPS33HW_ID ) {
        ESP_LOGE(TAG, "Sensor connection failed - retry so error can be seen.");
        /*manage here device not found */
        this->mark_failed();
        return;
    }
    
    this->set_timeout(50, [this]() {
        /*
        *  Restore default configuration
        */
        uint8_t rst;
        lps33hw_reset_set(&dev_ctx, PROPERTY_ENABLE);
        do {
            lps33hw_reset_get(&dev_ctx, &rst);
        } while (rst);
        /*
        *  Enable Block Data Update
        */
        lps33hw_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
        /*
        * Set Output Data Rate
        */
        lps33hw_data_rate_set(&dev_ctx, LPS33HW_ODR_10_Hz);
    });

}
void PressionComponent::sleep() {

}
void PressionComponent::wake_up() {

}

}

}
