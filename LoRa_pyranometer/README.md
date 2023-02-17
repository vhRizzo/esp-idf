# LoRaWAN Pyranometer

Implementation of low-cost pyranometer using various IoT sensors which sends data through a LoRaWAN radio module using ESP-IDF v5.0.
It is intended to develop a product which will be attached to a photovoltaic module in order to estimate it's Performance Ratio.

## Devices used:

  - ESP32 NodeMCU: Microcontroller dev-kit used.
  - BPW34: Photodiode to measure irradiance.
  - DS18B20: Temperature sensor intender to measure the superficial temperature of a photovoltaic module.
  - NEO-6M: GPS module.
  - BME280: Temperature, humidity and pressure sensor to measure the environment conditions.
  - DSM501A: Dust sensor to try to predict if there is gathered dust on a photovoltaic module.
  - SMW-SX1262M0: LoRaWAN radio used to send the gathered data.
