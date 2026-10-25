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
#### 2026-10 — Firmware rework (ESPHome components)

Replaced the hand-written ESPHome components with official/community ones, keeping
the same functionality. Verified: config valid + full compile on ESPHome 2026.10.0.

- **Stepper** → official ESPHome `tmc2209` (+ its base component `tmc22xx`),
  vendored under `components/` from the ESPHome `dev` branch (2026.11.0-dev)
  until 2026.11.0 is released. Covers both driver modes we use: UART speed
  control (VACTUAL) with position counting on the `INDEX` pin, and StallGuard
  homing via `diag_pin` + `on_stall` + the `tmc2209.stallguard` action.
- **Lidar (both VL53L1X)** → community `vl53l1x_pololu`
  (https://github.com/jasonwbarnett/esphome-vl53l1x-pololu, Pololu port, native
  ESPHome I2C, continuous ranging + in-place freeze recovery). One instance per
  I2C bus (`MULTI_CONF`).
- **Removed**: `components/tmc2209` (custom), `components/vl53l1x` +
  `components/i2c_platform` (ST-API/I2C bridge), `libraries/VL53L1X` (ST ULD),
  and the unused `stepper_cover`.
- **Ported to YAML**: the custom `stepper.calibrate` action is now a script
  (`on_stall` + `stepper.report_position` + persisted globals `min_step`/`max_step`,
  two-phase open/close with a stall-settle guard); driver tuning moved to
  `esphome: on_boot` (`tmc2209.configure inverse_direction`, `tmc2209.currents
  standstill_mode: freewheeling + hold_current: 0`, `tmc2209.stallguard
  threshold + tcool_threshold`).
- **Lidar speed/acceleration** (the "helping to open/close when pushed" feature)
  was ported INTO the community component, and `enable_pin` (XSHUT GPIO25/26)
  support was added so the sensor is released from reset before `init()`.
  Candidate for an upstream PR to the community repo.
- **HA entity rename**: `door errors` / `trashcan errors` → `door range status` /
  `trashcan range status` (data-ready errors no longer exist; use range_status /
  recovery_count). Update any HA automations referencing the old entity ids.

**Backports** used to run the 2026.11 official `tmc2209` on ESPHome 2026.10.0
(each marked in the vendored code): `cv.key_requires`, `cg.add_pin`,
`MockObjEnum.options(prefix=)`, `uart.new_uart_component`/`cg.new_bus_component`,
`Component::status_comm_result()`, `UARTDevice::discard_available()`; plus YAML
`stepper.stop` → `stepper.set_target` (current_position) and `Sensor::raw_state`
→ `get_raw_state()`.

**TODO when ESPHome 2026.11.0 is released**
- Delete `components/tmc22xx` and `components/tmc2209` and drop them from
  `external_components` in `devices/pcb-trashcan/v1.yaml` — they become built-in.
  No further YAML change is needed; the backports disappear.
- Keep `components/vl53l1x_pololu` (community + our speed/acceleration/additions).
- Optionally revert the YAML stop-as-`set_target` back to `stepper.stop`.
- Validate on hardware: `calibrate` button, direction, `stallguard`/`tcool`,
  and both lidars reading correctly.
