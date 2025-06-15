#pragma once
#include <PubSubClient.h>
#include "Settings.h"
#include "Loggers/ILogger.h"

class MqttClient {
    private:
        ILogger* logger;
        PubSubClient* client;
        uint8_t sentState = -1;

        void callback(char* topic, byte* payload, unsigned int length);

    public:
        MqttClient(ILogger* logger, PubSubClient* pubSubClient);
        bool initialise(Settings* settings, MQTT_CALLBACK_SIGNATURE);
        void publish(String topic, String payload, boolean retained);
        bool publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);
        void loop();
        bool isConnected();
};