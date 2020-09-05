#ifndef PLANT_SENSOR_ESPHOME_H
#define PLANT_SENSOR_ESPHOME_H

#include "esphome.h"
#include <plant_sensor.h>

static const auto DEFAULT_PLANT_UPDATE_RANGE = 60; 


class Plant_sensor_esphome : public PollingComponent {
    public:

        static Plant_sensor_esphome& get(Application& App, int8_t ss, size_t update_s=DEFAULT_PLANT_UPDATE_RANGE) {
            static std::map<int8_t, Plant_sensor_esphome*> iv_instances;
            if (iv_instances.find(ss) == iv_instances.end()) iv_instances[ss] = new Plant_sensor_esphome(App, ss, update_s);
            return *iv_instances.find(ss)->second;
        };

        static Plant_sensor_esphome& registerRange(Application& App, int8_t min_ss, int8_t max_ss, size_t update_s=DEFAULT_PLANT_UPDATE_RANGE) {
            for (int8_t ss = min_ss; ss < max_ss; ss++) get(App, ss, update_s);
            return get(App, min_ss, update_s);
        }
        
        Sensor       iv_sensor_version;
        Sensor       iv_sensor_measure_average;
        Sensor       iv_sensor_min_s;
        Sensor       iv_sensor_max_s;
        Sensor       iv_sensor_min_humidity_percentage;
        Sensor       iv_sensor_max_humidity_percentage;
        Sensor       iv_watering_step_s;
        Sensor       iv_sensor_humidity_raw;
        Sensor       iv_sensor_humidity;
        Sensor       iv_sensor_temperature;
        BinarySensor iv_is_debug;
        BinarySensor iv_is_connected;
        TextSensor   iv_sensor_uid;
        TextSensor   iv_sensor_build_date;
        TextSensor   iv_sensor_build_time;
        TextSensor   iv_sensor_build_commit;

        void setup() override {

            iv_plant_p = new Plant_sensor(SPI, iv_ss);

            init();
        }


        void update() override {
            if (iv_plant_p->is_error()) {
                iv_plant_p->refresh_all();
                init();
            } else {
                update_dynamic();
            }
            iv_is_connected.publish_state(!iv_plant_p->is_error());
        }

    private:

        Plant_sensor* iv_plant_p;

        Application& App;
        int8_t iv_ss;
        std::string iv_name;
        bool iv_registered_sensors = false;

        Plant_sensor_esphome(Application& App, int8_t ss, size_t update_s) :
            PollingComponent(update_s * 1000),
            App(App),
            iv_ss(ss)
        {
            App.register_component(this);
        }

        void init() {
            if (iv_plant_p->is_error()) {
                status_set_error();
            } else {
                status_clear_error();

                iv_name = iv_plant_p->get_name();

                update_static();

                update_dynamic();

                register_sensors();
            }
        }

        void register_sensors() {
            if (iv_registered_sensors) {
                return;
            }
            iv_registered_sensors = true;

            App.register_sensor(&iv_sensor_version);
            App.register_sensor(&iv_sensor_measure_average);
            App.register_sensor(&iv_sensor_min_s);
            App.register_sensor(&iv_sensor_max_s);
            App.register_sensor(&iv_sensor_min_humidity_percentage);
            App.register_sensor(&iv_sensor_max_humidity_percentage);
            App.register_sensor(&iv_watering_step_s);
            App.register_sensor(&iv_sensor_humidity_raw);
            App.register_sensor(&iv_sensor_temperature);
            App.register_sensor(&iv_sensor_humidity);

            App.register_binary_sensor(&iv_is_debug);
            App.register_binary_sensor(&iv_is_connected);

            App.register_text_sensor(&iv_sensor_uid);
            App.register_text_sensor(&iv_sensor_build_date);
            App.register_text_sensor(&iv_sensor_build_time);
            App.register_text_sensor(&iv_sensor_build_commit);
        }

        void update_static() {

            iv_sensor_version.set_name(iv_name + " Version");
            iv_sensor_version.publish_state(iv_plant_p->get_version());

            iv_sensor_measure_average.set_name(iv_name + " Measure Average");
            iv_sensor_measure_average.publish_state(iv_plant_p->get_measure_average());

            iv_sensor_min_s.set_name(iv_name + " Min Soil Raw");
            iv_sensor_min_s.publish_state(iv_plant_p->get_min_s());

            iv_sensor_max_s.set_name(iv_name + " Max Soil Raw");
            iv_sensor_max_s.publish_state(iv_plant_p->get_max_s());

            iv_sensor_min_humidity_percentage.set_name(iv_name + " Min humidity");
            iv_sensor_min_humidity_percentage.set_unit_of_measurement("%");
            iv_sensor_min_humidity_percentage.publish_state(iv_plant_p->get_min_humidity_percentage());

            iv_sensor_max_humidity_percentage.set_name(iv_name + " Max humidity");
            iv_sensor_max_humidity_percentage.set_unit_of_measurement("%");
            iv_sensor_max_humidity_percentage.publish_state(iv_plant_p->get_max_humidity_percentage());

            iv_watering_step_s.set_name(iv_name + " Watering Step");
            iv_watering_step_s.set_unit_of_measurement("s");
            iv_watering_step_s.publish_state(iv_plant_p->get_watering_step_s());

            iv_sensor_humidity_raw.set_name(iv_name + " humidity raw");
            //iv_sensor_humidity_raw.set_icon();
            iv_sensor_humidity_raw.publish_state(iv_plant_p->get_soil_raw());

            iv_sensor_temperature.set_name(iv_name + " temperature");
            iv_sensor_temperature.set_unit_of_measurement("°C");
            iv_sensor_temperature.set_accuracy_decimals(1);
            iv_sensor_temperature.publish_state(iv_plant_p->get_temp_C());

            iv_sensor_humidity.set_name(iv_name + " humidity");
            iv_sensor_humidity.set_unit_of_measurement("%");
            iv_sensor_humidity.set_accuracy_decimals(1);
            //iv_sensor_humidity.set_icon();
            iv_sensor_humidity.publish_state(iv_plant_p->get_soil_p());

            iv_is_debug.set_name(iv_name + " Is Debug");
            iv_is_debug.publish_state(iv_plant_p->is_debug());

            iv_is_connected.set_name(iv_name + " Connected");
            iv_is_connected.publish_state(!iv_plant_p->is_error());

            iv_sensor_uid.set_name(iv_name + " UID");
            iv_sensor_uid.publish_state(iv_plant_p->get_uid());

            iv_sensor_build_date.set_name(iv_name + " Build Date");
            iv_sensor_build_date.publish_state(iv_plant_p->get_build_date());

            iv_sensor_build_time.set_name(iv_name + " Build Time");
            iv_sensor_build_time.publish_state(iv_plant_p->get_build_time());

            iv_sensor_build_commit.set_name(iv_name + " Build Commit");
            iv_sensor_build_commit.publish_state(iv_plant_p->get_build_commit());
        }

        void update_dynamic() {
            iv_plant_p->refresh_sensors();
            if (iv_plant_p->is_error()) {
                status_set_error();
            } else {
                iv_sensor_humidity_raw.publish_state(iv_plant_p->get_soil_raw());
                iv_sensor_humidity.publish_state(iv_plant_p->get_soil_p());
                iv_sensor_temperature.publish_state(iv_plant_p->get_temp_C());
            }
        }

};

#endif //PLANT_SENSOR_ESPHOME_H