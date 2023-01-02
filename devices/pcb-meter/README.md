# Gas and Electricity meter

This device measures the electricity from two Ferraries power meters and gas from a G4 RC1 and a G6 RC1.

I did not create a new PCB as this sensor is fairly trivial. Instead I used an ESP32 DevKit C board plus a level converter as the sensors are
5V (there are different opinions on how well the ESP32 handles 5V inputs so I did not want to risk it). The sensors are connected to the board
5V and their outputs connect to the level converted and then to GPIOs.

- [3D Printed cage and sensors](https://www.printables.com/model/356429)
- Parts:
  - Controller
    - 1x [ESP32 Dev Kit C V4 NodeMCU](https://www.amazon.de/dp/B08BZFW41S)
    - [4x channel 5V to 3.3V level converter](https://www.ebay.de/itm/164730444736)
    - USB power supply - very low consumption so any would do
    - 1m USB power cable
    - 4x [3 pin JST connectors](https://www.amazon.de/dp/B082ZLYRRN)
    - Generic PCB board to hold all the above together
  - 2x Gas sensor
    - 1x [LJ18A3-8-Z/BX-5V](https://www.amazon.de/dp/B09BZ4FCF8)
    - 1x M4 20mm screw
  - 2x Electricity sensor
    - 1x [TCRT5000](https://www.amazon.de/dp/B09M7Z15FG)
    - 1m 3 wires cable
    - 3 pin connector
- PINs:
  - 19 - Electricity meter 1
  - 18 - Gas meter 1
  -  5 - Electricity meter 2
  - 17 - Gas meter 2
- Firmware: [ESPHome](https://esphome.io)
  - You need to modify/create your own esphome definition and include the yaml in this folder matching your PCB
  - [Example for v1.0](../../m1-casa96-meter.yaml)

# Changelog

## v1.0
  - First version
  - BUGS:
    - None so far
