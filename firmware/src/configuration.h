#pragma once

#include <Arduino.h>

#define WIFI_SSID "<PUT HERE YOUR WIFI SSID>"
#define WIFI_PASSWORD "<PUT HERE YOUR WIFI PASSWORD>"

#define MQTT_SERVER_HOST "<PUT HERE YOUR MQTT SERVER DOMAIN or IP>"
#define MQTT_SERVER_PORT 1883
#define MQTT_SERVER_USERNAME NULL // You can provide username for MQTT server if needed
#define MQTT_SERVER_PASSWORD NULL // You can provide password for MQTT server if needed
#define MQTT_CLIENT_TOPIC "wifi2mqtt/air_recuperation"
#define MQTT_CLIENT_ID "BelimoCRAB1P"

#define RECUPERATION_SWITCH_PIN 33
#define RECUPERATION_LED_LOW_PIN A3
#define RECUPERATION_LED_COMF_PIN A2
#define RECUPERATION_LED_HIGH_PIN A4
#define RECUPERATION_SWITCH_DELAY 50

#define MCU_DEFAULT_LOOP_DELAY 100