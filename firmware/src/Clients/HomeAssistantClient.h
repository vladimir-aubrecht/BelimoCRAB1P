#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "Loggers/ILogger.h"
#include "Recuperation.h"
#include <PubSubClient.h>

class HomeAssistantClient
{
    private:
        String FW_VERSION = "1.1.0-ha";        // bump your firmware version
        String baseTopic = "";

        Recuperation* recuperation;
        ILogger* logger;
        PubSubClient* client;

        String getChipId();

    public:
        HomeAssistantClient(Recuperation* recuperation, ILogger* logger, PubSubClient* pubSubClient);

        void publishDiscoveryConfig(String baseTopic);
        void receiveStates(char* topic, byte* payload, unsigned int len);
        void publishStates(uint8_t speedState);
};