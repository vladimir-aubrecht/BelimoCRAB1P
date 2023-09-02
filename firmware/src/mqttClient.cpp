#include "MqttClient.h"
#include <ArduinoJson.h>

MqttClient::MqttClient(Recuperation* recuperation, ILogger* logger)
{
    this->recuperation = recuperation;
    this->logger = logger;
    this->wiFiClient = new WiFiClient();
    this->client = new PubSubClient(*this->wiFiClient);
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length) {
    char* message = (char*) payload;

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        this->logger->error("Failed to parse JSON data: ");
        this->logger->error(error.c_str());
        return;
    }
    
    uint8_t source = doc["source"];

    if (source == 0) {
        return;
    }

    uint8_t selectedState = doc["air_flow_state"];

    recuperation->setState(selectedState);
}

bool MqttClient::initialise(Settings* settings)
{
    this->mqttTopic = settings->mqttClientTopic.c_str();    
    this->client->setServer(settings->mqttServerHost.c_str(), settings->mqttServerPort);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    this->client->setCallback(std::bind(&MqttClient::callback, this, _1, _2, _3));

    int8_t mqttRetryCount = 0;
    for (mqttRetryCount = 0; mqttRetryCount < 10 && !this->client->connected(); mqttRetryCount++) {
        this->logger->debug("Connecting to MQTT server...");
        if (this->client->connect(settings->mqttClientId.c_str(), settings->mqttServerUsername.c_str(), settings->mqttServerPassword.c_str() )) {
            this->logger->debug("Connected to MQTT server");
            this->client->subscribe(settings->mqttClientTopic.c_str());
        }
        else {
            this->logger->error("Failed with state ");
            this->logger->error(String(this->client->state()));
            delay(2000);
        }
    }

    if (mqttRetryCount >= 10) {
        return false;
    }

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
       
        char payload[41];
        snprintf(payload, 41, "{\"air_flow_state\": %d, \"source\": 0}", state);

        this->logger->debug("Sending payload:");
        this->logger->debug(payload);
        client->publish(this->mqttTopic, payload);
        this->logger->debug(payload);
    }

    client->loop();
}