#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "Loggers/ILogger.h"
#include "Recuperation.h"
#include "mqttClient.h"

class HomeAssistantClient
{
    private:
        String FW_VERSION = "1.1.0-ha";        // bump your firmware version

        Recuperation* recuperation;
        ILogger* logger;
        MqttClient* client;
        Settings* settings;

        String getChipId();

    public:
        HomeAssistantClient(Recuperation* recuperation, ILogger* logger, MqttClient* client, Settings* settings);

        void publishDiscoveryConfig();
        void receiveStates(char* topic, byte* payload, unsigned int len);
        void publishStates(uint8_t speedState);
};