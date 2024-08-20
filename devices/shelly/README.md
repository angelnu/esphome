# Links on how to program Shellies

- Shelly firmware to Tasmota OTA: https://github.com/tasmota/mgos32-to-tasmota32
  - NOTE: directly install esphome afterwards WITOUT applying any further Tasmota updates
- Downloading the Shelly firmware: TBD
- add additional ernergy monitoring: https://github.com/dentra/esphome-components/tree/master/components/energy_monitoring
- add Energy Statistics: https://github.com/dentra/esphome-components/tree/master/components/energy_statistics
- Get the latest original Shelly firmware by calling update API at https://updates.shelly.cloud/update/<device name>. [Example for Pro4PM](https://updates.shelly.cloud/update/Pro4PM)

## Shelly Pro 4PM

- Serial port pins (starting at the bottom, where the J8 label): 1.ground 2. GPIO0 3.remains empty 4.VDD 5.TX from programmer 6.RX from programmer
  - Programing pins can be accessed without opening the whole box, just by lifting the cover on top the LAN port
  - ![Photo](https://user-images.githubusercontent.com/133048788/237196200-69563d15-e127-4e95-bd16-67a1d65702f1.jpg)
- [using Ethernet port](https://community.home-assistant.io/t/ethernet-and-wifi-in-same-time-not-possible/405358)
- [display esphome config example](https://www.smarthomejetzt.de/esphome-mit-oled-display-fuer-die-anzeige-von-iobroker-mqtt-daten/)

### GPIO pins

Obtained from Tasmota

| GPIO | Code | Name |
|------|------|------|
| GPIO00 | 0 | Display CS |
| GPIO01 | 6210 | OptionA3 |
| GPIO02 | 0 | Display DC |
| GPIO03 | 6214 | OptionA7 |
| GPIO04 | 9568 | ADE7953 CS1 |
| GPIO05 | 0 | None |
| GPIO09 | 0 | None |
| GPIO10 | 0 | None |
| GPIO12 | 0 | Display Backlight |
| GPIO13 | 0 | Display MOSI |
| GPIO14 | 9569 | ADE7953 CS2 |
| GPIO15 | 0 | Display CLK |
| GPIO16 | 10272 | MCP23S17 CS |
| GPIO17 | 0 | ETH CLK? |
| GPIO18 | 5600 | ETH MDIO |
| GPIO19 | 0 | ETH TXD0? |
| GPIO20 | 0 | None |
| GPIO21 | 0 | ETH TXEN? |
| GPIO22 | 0 | ETH TXD1? |
| GPIO23 | 5568 | ETH MDC |
| GPIO24 | 0 | None |
| GPIO25 | 0 | ETH RXD0? |
| GPIO26 | 0 | ETH RXD1? |
| GPIO27 | 0 | ETH CRS_DV? |
| GPIO6 | 0 | None |
| GPIO7 | 0 | None |
| GPIO8 | 0 | None |
| GPIO11 | 0 | None |
| GPIO32 | 736 | SPI CLK |
| GPIO33 | 704 | SPI MOSI |
| GPIO34 | 3461 | ?? |
| GPIO35 | 10240 | MCP23S17 common interrupt |
| GPIO36 | 4736 | ADC Temp |
| GPIO37 | 0 | None |
| GPIO38 | 0 | None |
| GPIO39 | 672 | SPI MISO |