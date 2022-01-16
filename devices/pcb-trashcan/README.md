# Automated Trashcan 
This PCB allows controlling a trashcan so that it moves automatically with a motor (stepper) when
the door opens or the trashcan is pushed.

It uses lidars to detect the door and trashcan positions and a stepper with Stall Guard to detect
collisions.

TBD: picture

## Controller

- Aisler PCB project - Not available - as it was a one time model I used wire-wrapping to build the circuit (see pictures)
- [KiCad schematics](kicad/trashcan/trashcan.kicad_sch)
  - ![Schematics Preview](pictures/controller_schematics_v1.png)
  - TBD: screenshot
- Prototype PCB done with Wire Wrapping
  - TBD: pictures
- [Printed parts modelled in Fusion 360](https://a360.co/3F76WmE)
  - TBD: screenshot
  - TBD: fusion 360 archive and stl for parts
- Parts:
  - 1x [ESP32-WROOM-32](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
  - 1x [S2209 v3.1 TMC2209 driver controler](https://wiki.fysetc.com/Silent2209/#v31)
    - Ordered from [Amazon](https://www.amazon.de/gp/product/B08RDKS9N8)
  - 1x Stepper Motor Nema 17 1.5A 1.8°4
    - Ordered from [Amazon](https://www.amazon.de/gp/product/B07MCXKW68)
  - 1m GT2 timing belt and 2x pullys 
    - Ordered from [Amazon](https://www.amazon.de/gp/product/B07QNWJ1XC)
  - 2x lidar [vl53l1x](https://www.st.com/en/imaging-and-photonics-solutions/vl53l1x.html)
    - Ordered some with cover from [China](https://de.aliexpress.com/item/4000110308676.html)
    - Alternative: [Melopero board](https://www.melopero.com/shop/sensori/prossimita/melopero-vl53l1x-time-of-flight-long-distance-ranging-sensor-breakout/) more expensive but available in Amazon.
  - 2x 6 pin conector for lidars
  - 2m flatcable for lidars
  - 1x IC power switch [MCZ33996EK](https://www.nxp.com/docs/en/data-sheet/MC33996.pdf)
  - 1x DC 5,5 x 2,1MM Power Jack
    - Ordered from [Amazon](https://www.amazon.de/gp/product/B093FTFZ8Q)
- PINs:
  - Serial programmer:
    1. VCC
    2. RX
    3. TX
    4. GND
 - Power
 - 6 pins connector for trashcan lidar
 - 6 pins connector for door lidar
- Firmwares:
  - [esphome](esphome.io)
    - You need to modify/create your own esphome definition and include the yaml in this folder matching your PCB
    - [Example for v1](../../trashcan.yaml)

### Changelog

#### v1.0
  - First version
  - BUGS:
    - TBD