#include "Settings.h"

Settings::Settings(String defaultWifiSSID, String defaultWifiPassword, String defaultServerHost, uint16_t defaultServerPort, String defaultMqttServerUsername, String defaultMqttServerPassword, String defaultMqttClientId, String defaultMqttClientTopic)
{
    this->defaultWifiSSID = defaultWifiSSID;
    this->defaultWifiPassword = defaultWifiPassword;
    this->defaultMqttServerHost = defaultServerHost;
    this->defaultMqttServerPort = defaultServerPort;
    this->defaultMqttServerUsername = mqttServerUsername;
    this->defaultMqttServerPassword = mqttServerPassword;
    this->defaultMqttClientId = defaultMqttClientId;
    this->defaultMqttClientTopic = defaultMqttClientTopic;

    this->preferences = new Preferences();
}

void Settings::read()
{
    this->preferences->begin("credentials", true); 
    this->wifiSSID = preferences->getString("wifiSSID", this->defaultWifiSSID);
    this->wifiPassword = preferences->getString("wifiPassword", this->defaultWifiPassword);
    this->mqttServerHost = preferences->getString("mqttServerHost", this->defaultMqttServerHost);
    this->mqttServerPort = preferences->getUShort("mqttServerPort", this->defaultMqttServerPort);
    this->mqttServerUsername = preferences->getString("mqttServerUsername", this->defaultMqttServerUsername);
    this->mqttServerPassword = preferences->getString("mqttServerPassword", this->defaultMqttServerPassword);
    this->mqttClientId = preferences->getString("mqttClientId", this->defaultMqttClientId);
    this->mqttClientTopic = preferences->getString("mqttClientTopic", this->defaultMqttClientTopic);
    this->preferences->end();

}

void Settings::store()
{
    this->preferences->begin("credentials", false); 
    preferences->putString("wifiSSID", this->wifiSSID);
    preferences->putString("wifiPassword", this->wifiPassword);
    preferences->putString("mqttServerHost", this->mqttServerHost);
    preferences->putUShort("mqttServerPort", this->mqttServerPort);
    preferences->putString("mqttServerUsername", this->mqttServerUsername);
    preferences->putString("mqttServerPassword", this->mqttServerPassword);
    preferences->putString("mqttClientId", this->mqttClientId);
    preferences->putString("mqttClientTopic", this->mqttClientTopic);
    this->preferences->end();
}