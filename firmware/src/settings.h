#pragma once
#include <Preferences.h>

class Settings
{
    private:
        Preferences* preferences;
        String defaultWifiSSID;
        String defaultWifiPassword;
        String defaultMqttServerHost;
        uint16_t defaultMqttServerPort;
        String defaultMqttServerUsername;
        String defaultMqttServerPassword;
        String defaultMqttClientId;
        String defaultMqttBaseTopic;
        String defaultMqttAvaibilityTopic;
        String defaultMqttSubscribeTopic;

    public:
        String wifiSSID;
        String wifiPassword;
        String mqttServerHost;
        uint16_t mqttServerPort;
        String mqttServerUsername;
        String mqttServerPassword;
        String mqttClientId;
        String mqttBaseTopic;
        String mqttAvaibilityTopic;
        String mqttSubscribeTopic;

        Settings(String defaultWifiSSID, String defaultWifiPassword, String defaultMqttServerHost, uint16_t defaultMqttServerPort, String defaultMqttServerUsername, String defaultMqttServerPassword, String defaultMqttClientId, String defaultMqttBaseTopic, String defaultMqttAvaibilityTopic, String defaultMqttSubscribeTopic);

        void read();
        void store();
};