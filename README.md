# esphome
This repository contains my devices based on [ESPHome](https://esphome.io/). Each yaml in this repository represents one of my devices and can be programed following the instructions bellow.

The [devices](devices) folder contains the instructions for each device (firmware, schematics, 3D boxes, etc).

## Programming
To program/update these devices you need to:
1. create a `secrets.yaml` file. You can use `secrets_template.yaml` as base
2. start the [esphome dashboard](https://esphome.io/guides/getting_started_command_line.html#bonus-esphome-dashboard) with this git repository as config folder.
3. Connect to the dashboard 
4. First time you program your device you will need to program via a serial port. You might use a [CP2104](https://www.amazon.de/gp/product/B01CYBHM26/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)