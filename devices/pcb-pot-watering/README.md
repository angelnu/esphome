# Pot watering
This PCB allows watering pots with individual sensor and pumpes. The main feature is that the settings are stored in the sensor so you can disconned a pot from the controller and move with its sensor to another contoller: the watering settings will be transported.

TBD: more explanations

## Controller

- [Aisler PCB project](TBD)
- [KiCad schematics](TBD)
  - ![Schematics Preview](TBD)
- [KiCad PCB](TBD)
  - ![Schematics Preview](TBD)
- Parts:
  - 1x [ESP32-WROOM-32](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
  - TBD
- PINs:
  - TBD
- Firmwares:
  - [esphome](esphome.io)
    - You need to modify/create your own esphome definition and include the yaml in this folder matching your PCB
    - [Example for v1.1](../../w1-test.yaml)
  - [platformio](platformio.io) For testing without esphome you can use platformio to build and standalone test firmware that will periodically refresh the sensor data and print it to serial
    1. install [platformio](platformio.io)
    2. open a terminal at the [controller folder](controller)
    3. connect your ep32 via serial to your computer
    4. ensure your serial adapter matches the monitoring and programming sections in the [platformio config file](controller/platformio.ini).
    5. run `pio upload`

### Changelog

#### v1.1
  - First version
  - BUGS:
    - TBD


## Sensor
- [Aisler PCB project](https://aisler.net/p/HRCGITXS)
- [KiCad schematics](sensor/KiCad/SoilSensor_spi.sch)
  - ![Schematics Preview](TBD)
- [KiCad PCB](sensor/KiCad/SoilSensor_spi.kicad_pcb)
  - ![Schematics Preview](TBD)
- Parts:
  - TBD
- PINs:
  - TBD
- Firmware: [platformio](platformio.io)
  1. install [platformio](platformio.io)
  1. open a terminal at the [sensor folder](sensor)
  2. connect your sensor board via [STLink](https://www.st.com/en/development-tools/st-link-v2.html) to your computer
  3. run `pio upload`
- [3D Printed cage](cages):
  - TBD

### Changelog

#### v1.1
  - First version
  - BUGS:
    - TBD
