#pragma once
#include "Recuperation.h"
#include <PubSubClient.h>
#include "Settings.h"
#include "Loggers/ILogger.h"
#include "HomeAssistantClient.h"

class MqttClient {
    private:
        Recuperation* recuperation;
        ILogger* logger;
        PubSubClient* client;
        uint8_t sentState = -1;
        const char* mqttTopic = NULL;
        HomeAssistantClient* haClient;

        void callback(char* topic, byte* payload, unsigned int length);

    public:
        MqttClient(Recuperation* recuperation, ILogger* logger, PubSubClient* pubSubClient, HomeAssistantClient* haClient);
        bool initialise(Settings* settings);
        void processState(uint8_t state);
        bool isConnected();
};