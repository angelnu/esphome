# Thermostate for 220v
This PCB allows controlling two relays based on 1 wire Dallas temperature sensors. The board has two 1-wire buses - one for the in-board sensor and one for the external sensor(s).
- ![Thermostate_PCB_full_view](pictures/Temp_Actor_v1_full_view.jpg)

- [Aisler PCB project](https://aisler.net/p/BQLUSDIS)
- [KiCad PCB](kicad/pcb-power-usb/pcb-power-usb.kicad_pro)
  - ![Schematics Preview](pictures/TBD.png)
  - ![PCB Preview](pictures/TBD.png)
  - ![PCB front Preview](pictures/TBD.jpg)
  - ![PCB rear Preview](pictures/TBD.jpg)
- [3D Printed cage](cage):
  - [Fusion 360 online view](https://a360.co/3qWptNk)
  - [Fusion 360 archive](TBD)

- Parts:
  - 1x [ESP32-WROOM-32](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
  - 1x [AMS1117 - 3,3v 1A regulator in SOT-223 format](http://www.advanced-monolithic.com/pdf/ds1117.pdf)
  - 8x [IRLML0030 - 5A N-MOSFET](https://www.infineon.com/dgdl/Infineon-IRLML0030-DataSheet-v01_01-EN.pdf?fileId=5546d462533600a401535664773825df)
  - 8x [USB_A_Stewart_SS-52100-001_Horizontal](https://www.digikey.de/en/products/detail/stewart-connector/SS-52100-001/7902377)
  - 8x 0805 LEDs
  - 8x 0805 LED resistors (depends on selected LED)
  - 1x 10 KOhm resistor (reset)
  - 9x 0805 10 uF capacitor
  - 1x DC 5.5 x 2.1 mm barrel connector
    - Bought from [Amazon](https://www.amazon.de/gp/product/B093FTFZ8Q)
- PINs:
  - Header top left - external temperature sensors. From top to bottom:
    - VCC
    - Data
    - GND
- Firmware: [esphome](esphome.io)
  - You need to modify/create your own esphome definition and include the yaml in this folder matching your PCB
  - [Example for v1.0](../../s13_legos_shelves.yaml)

# Changelog

## v1.0
  - First version
  - BUGS:
    - None so far
