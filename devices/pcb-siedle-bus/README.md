
# Siedle In-Home bus client

This PCB allows interacting with a Sidle In-Home bus and automatically trigger actions or connect it to home automation such as [Home Assistant](https://www.home-assistant.io/). It uses [ESPHome](https://esphome.io) as base firmware.

- ![Overview picture](pictures/overview.png)
- [Aisler PCB project](https://aisler.net/p/WQLXACYH)
- <details>
  <summary>KiCad PCB - schematics and assembly </summary>

  - [KiCad PCB project](kicad/siedle-bus/siedle-bus.kicad_pro)
  - ![Schematics Preview](pictures/schematics.png)
  - ![PCB front Assembly](pictures/pcb-top-preview.jpg)
  - ![PCB tracks](pictures/pcb-tracks.png)
  - ![PCB front Assembly](pictures/pcb-top.png)
  - ![PCB front Assembly](pictures/pcb-bottom-preview.jpg)
  - ![PCB back Assembly](pictures/pcb-bottom.png)
  
  </details>
- <details>
  <summary>3D Printed Box </summary>

  - [3D Printed box](FreeCAD)
  - [FreeCad file](FreeCAD/siedle-bus.FCStd)
  - [3MF file](FreeCAD/siedle-bus-cage.3mf)
    ![cage-render](pictures/cage-preview.png)

  </details>
- <details>
  <summary>Parts</summary>

  - 1x [ESP32-WROOM-32 with 8MB Flash](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
  - TBD
  
  </details>
- PINs:
  - Bus connector - external temperature sensors. From top to bottom:
    - TA-
    - Ta+
  - Serial programming:
    - 3.3V
    - TX
    - RX
    - GND
- Firmware: [ESPHome.io](https://esphome.io)
  - You need to modify/create your own ESPHome definition and include the yaml in this folder matching your PCB
  - [Example for v1.0](../../interphone_small_flat.yaml)

## Bus information

The bus uses only 2 wires for communication and powering devices. A third wire is also present but not used here.

The bus provides 27V and can power client devices for up to 15mA (in the original board circuit there is a current limiter). It is very important to ensure that a high impedance is present to AC to not affect the bus communication. This is achieved in this project using a [Girator](https://en.wikipedia.org/wiki/Gyrator). The girator in this project uses a MOSFET to implement the girator and implement a virtual coil.

To prepare the bus to send a message, the bus is first shorted with 200 Ohms. This triggers the power supply of the bus to lower the current and allow the bits of the message to be sent. To send the bits a resistor of 10 ohms in parallel to the 200 Ohms is used to signal a zero while the 200 Ohms signal a 1. Each bit is 2 ms.

The PCB in this project uses a PNP Bipolar transistor to detect the bus carrier (when the voltage is dropped) and the following message bits. An NPN MOSFET is used to add the resistors to the bus.

### Message

Each message sent in the bus has 32 bits:

- 31-29: 010
- 28-23: command
- 22-18: destination
- 17-14: destination bus
- 13-12: 010 (first bit might be 1)
- 10-06: source
- 05-02: source bus
- 01-00: 00 (first bit might be 1 - broadcast?)

### References

I used all the reverse engineering from this [mikrocontroller Forum thread (german)](https://www.mikrocontroller.net/topic/308271) as base. Thanks a lot for all that contributed there.

Example of devices using this bus:
- [aib-150-01 Vendor page](https://www.siedle.de/de-de/home/service/linked-pages/produktkatalog/tuersprechanlagen-innen/siedle-basic/aib-150-01/)

## Changelog

### v1

- First version