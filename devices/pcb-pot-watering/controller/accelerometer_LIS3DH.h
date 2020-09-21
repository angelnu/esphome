#pragma once

#include "esphome.h"
namespace esphome {

namespace LIS3DH {


#include "lis3dh_reg.h"

class AcceleratorComponent: public PollingComponent, public i2c::I2CDevice {
    public:
        
        AcceleratorComponent() : PollingComponent(2000) {};
        void setup() override;
        void dump_config() override;
        float get_setup_priority() const override;
        void update() override;
        void soft_reset();
        void sleep();
        void wake_up();
 
    protected:
        sensor::Sensor *g_x = new Sensor();
        sensor::Sensor *g_y = new Sensor();
        sensor::Sensor *g_z = new Sensor();
        sensor::Sensor *temperature = new Sensor();
    
    private:
        stmdev_ctx_t dev_ctx;


};




static const char *TAG = "LIS3DH";

static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len){
    AcceleratorComponent* parent = (AcceleratorComponent*)(handle);
    reg |= 0x80; //turn auto-increment bit on, bit 7 for I2C
    return !parent->write_bytes(reg, bufp, len);
}
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len){
    AcceleratorComponent* parent = (AcceleratorComponent*)handle;
    reg |= 0x80; //turn auto-increment bit on, bit 7 for I2C
    return !parent->read_bytes(reg, bufp, len);
}
 
 
void AcceleratorComponent::setup() {
    ESP_LOGCONFIG(TAG, "Setting up LIS3DH...");

    //this->set_i2c_address(LPS33HWTR_I2C_ADD);
    /*
    *  Initialize mems driver interface
    */
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = (void*)this; 
    
    this->wake_up();
    this->soft_reset();

    
    ESP_LOGCONFIG(TAG, "Setting up LIS3DH done");
}

void AcceleratorComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "LIS3DH:");
    LOG_I2C_DEVICE(this);
    if (this->is_failed()) {
        ESP_LOGE(TAG, "Communication with LIS3DH failed!");
        this -> setup();
    }
    LOG_UPDATE_INTERVAL(this);
  
    LOG_SENSOR(" ", "X", this->g_x);
    LOG_SENSOR(" ", "Y", this->g_y);
    LOG_SENSOR(" ", "Z", this->g_z);
    LOG_SENSOR(" ", "Temperature", this->temperature);
}

float AcceleratorComponent::get_setup_priority() const { return setup_priority::DATA; }
void AcceleratorComponent::update() {
    if (this->status_has_warning()) {
        ESP_LOGW(TAG, "Retrying to reconnect the sensor.");
        this->soft_reset();
    }

    this->wake_up();
 
    this->set_timeout(50, [this]() {

        // Read acceleration output until new value available
        uint8_t data_ready=0;
        while(!data_ready) assert(lis3dh_xl_data_ready_get(&dev_ctx, &data_ready) == 0);
        
        /* Read accelerometer data */
        uint16_t data_raw_acceleration[3] = {0};
        assert(lis3dh_acceleration_raw_get(&dev_ctx, (uint8_t*)data_raw_acceleration) == 0);

        // Convert data to g
        float x = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[0])/1000;
        float y = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[1])/1000;
        float z = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[2])/1000;

        ESP_LOGD(TAG, "Got acceleration from %#04x: (%.4f,%.4f,%.4f) g", address_, x, y, z);
        this->g_x->publish_state(x);
        this->g_y->publish_state(y);
        this->g_z->publish_state(z);

        // Read acceleration output until new value available
        data_ready=0;
        while(!data_ready) assert(lis3dh_temp_data_ready_get(&dev_ctx, &data_ready) == 0);

        /* Read temperature data */
        uint16_t data_raw_temperature = 0;
        assert(lis3dh_temperature_raw_get(&dev_ctx, &data_raw_temperature) == 0);

        // convert data to °C
        float temperature_degC = lis3dh_from_lsb_hr_to_celsius(data_raw_temperature);

        ESP_LOGD(TAG, "Got temperature from %#04x: %.2f °C", address_, temperature_degC);
        this->temperature->publish_state(temperature_degC);

        this->status_clear_warning();
        this->sleep();
    });
}

void AcceleratorComponent::soft_reset() {

    /*
    *  Check device ID
    */
    uint8_t whoamI = 0;
    lis3dh_device_id_get(&dev_ctx, &whoamI);
    if ( whoamI != LIS3DH_ID ) {
        ESP_LOGE(TAG, "Sensor connection failed - retry so error can be seen.");
        /*manage here device not found */
        this->mark_failed();
        return;
    }

    /* Enable Block Data Update. */
    assert(lis3dh_block_data_update_set(&dev_ctx, PROPERTY_ENABLE) == 0);

    /* Set Output Data Rate to 1Hz. */
    assert(lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_1Hz) == 0);

    /* Set full scale to 2g. */
    assert(lis3dh_full_scale_set(&dev_ctx, LIS3DH_2g) == 0);

    /* Enable temperature sensor. */
    assert(lis3dh_aux_adc_set(&dev_ctx, LIS3DH_AUX_ON_TEMPERATURE) == 0);

    /* Set device in continuous mode with 12 bit resol. */
    assert(lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit) == 0);

}
void AcceleratorComponent::sleep() {

}
void AcceleratorComponent::wake_up() {

}

}

}
