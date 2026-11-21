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

#### 2026-10 — Motion control: direction fix, lidar-based stop, calibration

On-hardware follow-up hardening of the reworked config (all tuning values live in
`v1.yaml` under `substitutions:`).

- **Direction fix**: the old custom component's `inverted: true` and the official
  component's `inverse_direction: true` have OPPOSITE polarity. With
  `inverse_direction: true` a positive target drove the bucket IN, so "door open"
  jammed it against the closed stop and it never came out. Use
  `inverse_direction: false` (+ = out).
- **Position comes from the trashcan lidar, not the step count**: the belt slips
  under load, so the driver's INDEX count drifts from the real bucket position.
  All motion logic is driven by the lidar distance; the motor position is derived
  as `(dist - dist_in) * (max_step - min_step) / (dist_out - dist_in)` and is
  re-computed at the start of every move (this also handles a bucket pushed by
  hand).
- **Fast trashcan lidar**: the end-of-travel stop needs a fresh distance, so the
  trashcan VL53L1X runs with a short integration budget (`vl53_tc_timing_budget`
  50 ms / `vl53_tc_inter_measurement` 60 ms, `vl53_tc_update_interval` 30 ms); the
  door lidar keeps the slower default settings.
- **Smooth early stop, no over-drive**: while a move runs, a 100 ms interval
  re-aims the motor target at `current + dir * distance_still_to_go` (from the
  lidar) so the stepper's built-in deceleration (`deceleration: 400`) brings it
  smoothly to a stop exactly at the limit. Within `trashcan_end_margin_mm` (20 mm,
  headroom left for a heavier bucket) of the limit the CURRENT IS CUT (`tmc2209`
  disable) so the last few cm are done by momentum. The driver is also disabled
  whenever a move ends, so the bucket is free to be pushed by hand.
- **HARD SAFETY guards** (checked in the same 100 ms interval): a move is aborted -
  the current is cut and the driver disabled - if it runs longer than
  `move_timeout_ms` (7000 ms, i.e. slightly more than a normal open), or if it is
  an OUT move while the door is *really* closed (a VALID reading - `range_status`
  0 - below `door_close_distance` 200 mm). A garbled door reading with a bad range
  status is NOT treated as "closed", so it cannot abort a legitimate OUT.
- **"Door open" trigger (validity-based, edge-triggered)**: `open_trashcan` runs
  only on the rising edge of the `trashcan door open` template `binary_sensor`. The
  VL53L1X on the door bus glitches to small distances (0/50/250 mm) with an INVALID
  `range_status` even while the door is open. Those glitches must not be read as
  "closed": they used to toggle the sensor OFF->ON and re-trigger the bucket, and
  could also start a move with the door shut. So "closed" is defined as a VALID
  reading (`range_status` 0) at/below `door_close_distance`; **everything else is
  "open"**. 500 ms on/off delays reject single bad frames. The door lidar's
  `range_status` is exposed by the component (`get_range_status()`) for these
  checks. The trigger is edge-only (a rising edge), so while the bucket is out and
  the door stays open there is no re-trigger. (An earlier `current_position !=
  target_position` guard was removed: after a lidar-based cut the step target is
  not re-synced immediately, so that guard intermittently blocked a legitimate
  open.)
- **State re-sync**: whenever the bucket is at rest (no move in progress) the
  logical `trashcan_out` flag is re-derived from the trashcan lidar, so it cannot
  go stale after an interrupted/partial move and block the next open.
- **Manual `close_trashcan` button**: drives the bucket back IN (same action as
  "helping to close") - handy to retract the bucket by hand or over the API.
- **Belt/mechanical protection**: the same interval stops the motor if, while
  commanded, the INDEX position has not advanced for >500 ms, or the RAW lidar
  distance has not changed by >=2 mm for >1200 ms (belt slipping / jammed).
- **"Helping to close" is push-only and de-bounced**: a spurious trigger right
  after the bucket came out (noise at the stop) used to close it by itself. It now
  requires an inward speed above `trashcan_speed_min_delta` (0.15 m/s), the bucket
  to be out, and a short settle window (`close_ok_at`, 1 s after the OUT cut).
- **End-position calibration**: the `calibrate` button drives slowly (200 steps/s)
  to BOTH mechanical stops and records the real lidar distances into the persisted
  globals `dist_in` / `dist_out` (measured here: 87 / 491 mm); it also keeps
  `min_step` / `max_step` as coarse safety bounds. Re-run after any mechanical
  change.
- **Framework**: builds and runs on **ESP-IDF** (the ESPHome default for ESP32,
  so no `framework:` override is needed). The Arduino framework was only needed by
  the removed custom components; the official `tmc2209`/`tmc22xx` and the community
  `vl53l1x_pololu` are framework-agnostic. Verified on hardware (door -> out,
  push -> close).
- **Speed / acceleration**: `max_speed` 1000 steps/s; per-direction acceleration
  (`stepper_out_acceleration` 300, `stepper_in_acceleration` 1000) - gentle going
  out (heavy bucket, avoid belt slip), aggressive going in (the bucket is already
  moving when "helping to close" fires).

#### 2026-11 - ESPHome 2026.11.0: all components built-in (no vendored code)

2026.11.0 shipped both the `tmc2209` stepper and a new official `vl53l1x` lidar
component, so the vendored/community copies are gone and the device runs on
standard components only (no `external_components`).

- **Stepper** -> built-in `tmc2209`/`tmc22xx`. `components/tmc2209` and
  `components/tmc22xx` (the vendored official dev-branch copies) were deleted;
  the 2026.10 backports disappeared with them. No YAML change was needed - the
  vendored code was the official 2026.11 code.
- **Lidar (both VL53L1X)** -> built-in `vl53l1x` (esphome#13605). The community
  `vl53l1x_pololu` copy was deleted. Differences handled in the port:
  - **Speed** (critical for the "helping to close when pushed" feature): the
    official component has no speed sub-sensor, so it is now a `copy` sensor
    of the (internal, unfiltered) raw lidar that replicates the pololu math
    exactly - `(d - last_d)/dt` (mm/ms == m/s) over consecutive publishes and
    real elapsed time, with the baseline reset on an invalid (NaN) frame. The
    trigger threshold (`trashcan_speed_min_delta` 0.15 m/s) and conditions are
    unchanged. The `acceleration` sensor was already dropped (unused, it
    published on every frame) and is not ported.
  - The official component works in **single-shot mode**: one measurement and
    one publish per `update_interval` (not continuous ranging). The trashcan
    lidar still publishes every ~60 ms (30 ms interval, 50 ms budget), the
    door lidar every ~210 ms (100 ms interval, 200 ms budget) - same cadence
    as the pololu's inter-measurement settings.
  - **Unit**: the official component reports METERS; a `multiply: 1000` filter
    (plus `unit_of_measurement: "mm"`) keeps every threshold, the published
    entities and the persisted `dist_in`/`dist_out` calibration globals in mm
    - no recalibration needed after the OTA.
  - **Validity**: "frame not valid" is now `isnan(get_raw_state())` instead of
    `get_range_status() != 0`; all door-open/closed and safety lambdas were
    rewritten. An invalid frame is never "door closed" (it cannot abort an OUT
    move) and never "door open". The `door_very_open_distance` escape hatch (a
    flagged but far frame) has no equivalent - the official component has no
    distance on an invalid frame - and was removed; long mode reads a real
    open (~2 m) as a VALID value, so this should not matter.
  - **Lost entities** (no equivalent in the official component; update any HA
    automations referencing them): `door range status`, `trashcan range
    status`, `door recovery count`, `trashcan recovery count`, `door frames`,
    `trashcan frames`.
  - **Lost: the freeze watchdog** - a frozen lidar is no longer power-cycled
    over XSHUT; the official component recovers a hung measurement itself
    (NaN + restart at the next tick) but a wedged I2C bus stays wedged until
    reboot. Watch `door distance` going unavailable - if freezes return,
    revisit (e.g. pin the pololu component back or add a watchdog).
  - **Lost: `inter_measurement`** - not configurable on the official component.
- Dashboard image: k8s-gitops already bumped `esphome` to `2026.11.0`
  (renovate PR angelnu/k8s-gitops#6553) - the built-in components require it.
- Validated offline: `esphome config` (2026.11.0) valid; the speed/acceleration
  lambdas unit-tested against the pololu algorithm (frame sequences: static
  bucket, constant push, NaN gaps, baseline reset).
- **TODO on hardware**: flash + verify `calibrate` button, direction,
  `stallguard`/`tcool`, both lidars reading correctly, and above all the
  "helping to close" trigger: push the bucket gently while out and watch the
  `trashcan speed` entity (~-0.2 to -0.5 m/s) drive the bucket back in.
