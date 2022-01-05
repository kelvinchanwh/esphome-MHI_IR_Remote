# esphome-MHI_IR_Remote

Wirelessly control your MHI HVAC equipment with an ESP8266 or
ESP32 using the [ESPHome](https://esphome.io) framework.

## Usage
Below is an example configuration which will include wireless strength
indicators and permit over the air updates. You'll need to create a
`secrets.yaml` file inside of your `esphome` directory with entries for the
various items prefixed with `!secret`.

```yaml
substitutions:
  name: kelvin-ac
  friendly_name: Kelvin's Room AC
  owner: kelvin

esphome:
  name: ${name}
  platform: ESP8266
  board: nodemcuv2

# Enable logging
logger:

# Enable Home Assistant API
api:

ota:
  password: "edd39e76e44b8470095c4e78cd646044"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "${friendly_name} Fallback"
    password: !secret fallback_password

captive_portal:

# Example configuration entry
remote_transmitter:
  pin: D3
  carrier_duty_percent: 50%
  
# Sync time with Home Assistant.
time:
  - platform: homeassistant
    id: homeassistant_time
    
sensor:
  #DHT11 Sensor for Ambient Temp
  - platform: dht
    pin: D4
    temperature:
      name: "Kelvin's Room Temperature"
      id: ${owner}_room_temp
    humidity:
      name: "Kelvin's Room Humidity"
      id: ${owner}_room_humidity
    update_interval: 60s

  # Uptime sensor.
  - platform: uptime
    name: ${name} Uptime

  # WiFi Signal sensor.
  - platform: wifi_signal
    name: ${name} WiFi Signal
    update_interval: 60s
    
# Text sensors with general information.
text_sensor:
  # Expose ESPHome version as sensor.
  - platform: version
    name: ${name} ESPHome Version
  # Expose WiFi information as sensors.
  - platform: wifi_info
    ip_address:
      name: ${name} IP
    ssid:
      name: ${name} SSID
    bssid:
      name: ${name} BSSID
    
external_components:
  - source: github://kelvinchanwh/esphome-MHI_IR_Remote@develop
    refresh: 0s

climate:
  - platform: espmhi
    name: ${friendly_name}
    sensor: ${owner}_room_temp
    fan_default: "auto"
    horizontal_default: "swing"
    vertical_default: "up"
    

```