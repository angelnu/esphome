# Hiking DDS238-2 WIFI Single Phase 65A Energy Meter

Also sold in Germany as _KETOTEK Smart Electricity Meter DIN Rail 1-Phase Bi-Directional, Digital Energy Meter WLAN 5 (65) A 90-250 V, Two-Direction Counter 1 Phase, KWH Counter WLAN Remote Control by App_

[Amazon link](https://www.amazon.de/dp/B0B5KWHB4D)

[Esphome instructions](https://devices.esphome.io/devices/Hiking-DDS238-2-WIFI)

[Forum link](https://community.home-assistant.io/t/yaml-for-hiking-tomzn-tuya-dds238-2-wifi-din-rail-energy-meter/308007)

[Tasmota forum entry with pictures](https://github.com/krikk/Hiking-DDS238-2-WIFI-Din-Rail-Energy-Meter-flashing-Tasmota)

## How to flash

It needs to be programed via serial port, soldering cables to [TYWE3S chip](https://tasmota.github.io/docs/devices/TYWE3S/).

[Picure of soldered cables](https://user-images.githubusercontent.com/4527135/124747294-b04dd780-df19-11eb-962f-7e01ce873563.png) a picture

## data to reverse engineer

https://github.com/make-all/tuya-local/commit/eb28481d6d37c3865fc3d1ed4ba545ec49a5a2b8


https://developer.tuya.com/en/docs/iot/dlq?id=Kb0kidk9enyh8
https://github.com/tuya/tuya-home-assistant/issues/495

'''
if self._status_range.type == "Raw" and self.device.category == "dlq":
            raw = b64.b64decode(value)
            if self.device_class == DEVICE_CLASS_VOLTAGE:
                return struct.unpack('>H', raw[0:2])[0] / 10.0
            if self.device_class == DEVICE_CLASS_CURRENT:
                return struct.unpack('>L', b'\x00' + raw[2:5])[0] / 1000.0
            if self.device_class == DEVICE_CLASS_POWER:
                return struct.unpack('>L', b'\x00' + raw[5:8])[0]
'''

'''
    @classmethod
    def from_raw(cls, data: str) -> ElectricityTypeData:
        """Decode base64 string and return a ElectricityTypeData object."""
        raw = base64.b64decode(data)
        voltage = struct.unpack(">H", raw[0:2])[0] / 10.0
        electriccurrent = struct.unpack(">L", b"\x00" + raw[2:5])[0] / 1000.0
        power = struct.unpack(">L", b"\x00" + raw[5:8])[0] / 1000.0
        return cls(
            electriccurrent=str(electriccurrent), power=str(power), voltage=str(voltage)
        )

'''

https://github.com/tuya/tuya-home-assistant/issues/495



    Phase_a parsing rules:

        A-phase voltage, current and power
        Big-endian mode, HEX format, 10 bytes in total
        unit accuracy: voltage, 2 bytes, unit 0.1V. Current, 3 bytes, 0.001a. Phase A active power, 3 bytes, unit 0.001kW
        Packet format

    For example, 08 80 00 03 E8 00 27 10 indicates the A phase 217.6V, the A phase current 1.000A, and the A phase power
    10.000 KW

In my case the following state

{
  "code": "phase_a",
  "value": "CXQACcQAAWwAAA=="
},

can be decoded as 09 74 00 09 c4 00 01 6c 00 00, which means
0x974 = 242.0V
0x9C4 = 2.5A
0x16C = 364W

17 raw
====
00.00.00.00.00.00.00.00.00.00
64 100
00
01.0E 270 overvoltage protection (1V - 100-270)
00.00
AA undervoltage protection (1V - 90 to 250)
00.00.00.00.00
0A under/over voltage recovery (reset if overvoltage-this or undervoltage+this)
00.00.00.00.00.00

18 raw
====
00.05 5
00.64 100
00.05 5
00.1E 60 over/under voltage or overcurrent protection time (in 0.1 seconds - 0.1 to 60.0)
00.3C 30 recovery delay time (in seconds - 5 to 512)
00.00.00.00.00.00.00