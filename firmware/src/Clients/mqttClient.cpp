#include "MqttClient.h"
#include <ArduinoJson.h>

MqttClient::MqttClient(ILogger* logger, PubSubClient* pubSubClient)
{
    this->logger = logger;
    this->client = pubSubClient;
}

bool MqttClient::initialise(Settings* settings, MQTT_CALLBACK_SIGNATURE)
{
    this->client->setServer(settings->mqttServerHost.c_str(), settings->mqttServerPort);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    this->client->setCallback(callback);

    int8_t mqttRetryCount = 0;
    for (mqttRetryCount = 0; mqttRetryCount < 10 && !this->client->connected(); mqttRetryCount++) {
        this->logger->debug("Connecting to MQTT server...");

        if (this->client->connect(settings->mqttClientId.c_str(), settings->mqttServerUsername.c_str(), settings->mqttServerPassword.c_str(), settings->mqttAvaibilityTopic.c_str(), 0, true, "offline" ))
        {
            this->client->setBufferSize(1024);
            this->logger->debug("Connected to MQTT server");
            this->client->subscribe(settings->mqttSubscribeTopic.c_str());
        }
        else
        {
            this->logger->error("Failed with state ");
            this->logger->error(String(this->client->state()));
            delay(2000);
        }
    }

    if (mqttRetryCount >= 10) {
        this->logger->error("Did not manage to establish connection to mqtt after 10 tries.");
        return false;
    }
    
    return true;
}

bool MqttClient::isConnected()
{
    return client->connected();
}

void MqttClient::publish(String topic, String payload, boolean retained)
{
    this->client->publish(topic.c_str(), payload.c_str(), retained);
}

bool MqttClient::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained)
{
    this->client->publish(topic, payload, plength,retained);
}

void MqttClient::loop()
{
    client->loop();
}