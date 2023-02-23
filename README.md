# What is this project about?
This repository documents how to extend air recuperation controller Belimo CRA-B1P in a way you can control it through MQTT and plug it into your smart home.

# Wiring

Belimo CRA-B1P does not have any connector or anything which would allow to plug smart functionality and therefore you'll need to solder few wires:
- 2 wires to button - these are connected to relay and simulates button press. Relay is used as high voltage (12V) is going through the button.
- 3 wires to LEDs (one to each) - these are used to read voltage on LEDs to detect current state:
    - If no LED is on, controller rebooted.
    - If one of them is on, then it has voltage around 2V (code is using 1.6V as minimum to consider it on), otherwise it is around 0V.

Powering of microcontroller is not solved at this moment, meaning recommended way is to use battery or extra USB cable from somewhere. I believe it should be possible to power it up directly from Belimo CRA-B1P, but it will require extra components as Belimo CRA-B1P is powered up by 12V AC which is not compatible with common microcontrollers.