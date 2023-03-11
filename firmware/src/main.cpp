#include <Arduino.h>
#include "configuration.h"
#include "Drivers/relay.h"
#include "Drivers/ledReader.h"
#include "recuperation.h"
#include "mqttClient.h"
#include "configurationServer.h"
#include "settings.h"

Relay* relay;
LedReader* ledReader;
Recuperation* recuperation;
MqttClient* mqttClient;
ConfigurationServer* configurationServer = NULL;

#ifndef LED_BUILTIN
int LED_BUILTIN = 2;
#endif 

void setup() {
    Serial.begin(115200);
    pinMode (LED_BUILTIN, OUTPUT);

    relay = new Relay(RECUPERATION_SWITCH_PIN, RECUPERATION_SWITCH_DELAY, &Serial);
    ledReader = new LedReader(RECUPERATION_LED_LOW_PIN, RECUPERATION_LED_COMF_PIN, RECUPERATION_LED_HIGH_PIN);
    recuperation = new Recuperation(ledReader, relay, &Serial);
    mqttClient = new MqttClient(recuperation, &Serial);
    
    Settings* settings = new Settings(WIFI_SSID,  WIFI_PASSWORD, MQTT_SERVER_HOST, MQTT_SERVER_PORT, MQTT_SERVER_USERNAME, MQTT_SERVER_PASSWORD, MQTT_CLIENT_ID, MQTT_CLIENT_TOPIC);

    if (!mqttClient->initialise(settings))
    {
        configurationServer = new ConfigurationServer(settings, &Serial);
    }
    else
    {
        delay(10000);
        recuperation->setState(1);
        delay(1000);
        recuperation->setState(3);
    }
}

void loop() {
    if (configurationServer == NULL) {
        uint16_t selectedState = ledReader->readState();
        mqttClient->processState(selectedState);
    }

    delay(MCU_DEFAULT_LOOP_DELAY);
}