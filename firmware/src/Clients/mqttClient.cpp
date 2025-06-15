#include "MqttClient.h"
#include <ArduinoJson.h>

MqttClient::MqttClient(Recuperation* recuperation, ILogger* logger, PubSubClient* pubSubClient, HomeAssistantClient* haClient)
{
    this->recuperation = recuperation;
    this->logger = logger;
    this->client = pubSubClient;
    this->haClient = haClient;
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length) {
    this->haClient->receiveStates(topic, payload, length);
}

bool MqttClient::initialise(Settings* settings)
{
    this->mqttTopic = settings->mqttClientTopic.c_str();    
    this->client->setServer(settings->mqttServerHost.c_str(), settings->mqttServerPort);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    this->client->setCallback(std::bind(&MqttClient::callback, this, _1, _2, _3));

    String avaibilityTopic = settings->mqttClientTopic + "/status";

    int8_t mqttRetryCount = 0;
    for (mqttRetryCount = 0; mqttRetryCount < 10 && !this->client->connected(); mqttRetryCount++) {
        this->logger->debug("Connecting to MQTT server...");

        if (this->client->connect(settings->mqttClientId.c_str(), settings->mqttServerUsername.c_str(), settings->mqttServerPassword.c_str(), avaibilityTopic.c_str(), 0, true, "offline" )) {
            this->logger->debug("Connected to MQTT server");
            this->client->subscribe((settings->mqttClientTopic + "/fan/#").c_str());
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
    
    this->haClient->publishDiscoveryConfig(settings->mqttClientTopic);
    this->client->publish(avaibilityTopic.c_str(), "online", true);

    return true;
}

bool MqttClient::isConnected()
{
    return client->connected();
}

void MqttClient::processState(uint8_t state)
{
    if (this->sentState != state) {
        this->sentState = state;

        this->haClient->publishStates(state);
    }

    client->loop();
}