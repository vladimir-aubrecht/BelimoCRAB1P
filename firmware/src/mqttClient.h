#pragma once
#include "Recuperation.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "Settings.h"
#include "Loggers/ILogger.h"

class MqttClient {
    private:
        Recuperation* recuperation;
        ILogger* logger;
        WiFiClient* wiFiClient;
        PubSubClient* client;
        uint8_t sentState = -1;
        const char* mqttTopic = NULL;

        void callback(char* topic, byte* payload, unsigned int length);

    public:
        MqttClient(Recuperation* recuperation, ILogger* logger);
        bool initialise(Settings* settings);
        void processState(uint8_t state);
        bool isConnected();
};