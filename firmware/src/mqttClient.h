#pragma once
#include "recuperation.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "settings.h"

class MqttClient {
    private:
        Recuperation* recuperation;
        HardwareSerial* serial;
        WiFiClient* wiFiClient;
        PubSubClient* client;
        uint8_t sentState = -1;
        const char* mqttTopic = NULL;

        void callback(char* topic, byte* payload, unsigned int length);

    public:
        MqttClient(Recuperation* recuperation, HardwareSerial* serial);
        bool initialise(Settings* settings);
        void processState(uint8_t state);
};