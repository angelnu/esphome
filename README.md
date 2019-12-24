# esphome
This repository contains my settings for [ESPHome](https://esphome.io/).

IT requires a `secrets.yaml` file as:
```
wifi_ssid: "ssi"
wifi_password: "hidden_password"
wifi_ap_ssid: "esphome_${devicename}"
wifi_ap_password: "hidden_password"

api_password: "hidden_password"

ota_password: "hidden_password"

mqtt_broker: "url or ip
mqtt_port: 1883
mqtt_user: user
mqtt_password: "hidden_password"
```
