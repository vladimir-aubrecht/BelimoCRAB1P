#include "mqttClient.h"
#include <ArduinoJson.h>

MqttClient::MqttClient(Recuperation* recuperation, HardwareSerial* serial)
{
    this->recuperation = recuperation;
    this->serial = serial;
    this->wiFiClient = new WiFiClient();
    this->client = new PubSubClient(*this->wiFiClient);
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length) {
    char* message = (char*) payload;

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        this->serial->print("Failed to parse JSON data: ");
        this->serial->println(error.c_str());
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
    settings->read();
    this->mqttTopic = settings->mqttClientTopic.c_str();

    WiFi.begin(settings->wifiSSID.c_str(), settings->wifiPassword.c_str());
    int8_t wifiRetryCount = 0;
    for (wifiRetryCount = 0; wifiRetryCount < 10 && WiFi.status() != WL_CONNECTED; wifiRetryCount++) {
        delay(1000);
        this->serial->println("Connecting to WiFi...");
    }

    if (wifiRetryCount >= 10) {
        this->serial->println("Giving up connecting to wifi.");
        return false;
    }

    this->serial->println("Connected to WiFi");
    
    this->client->setServer(settings->mqttServerHost.c_str(), settings->mqttServerPort);

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    this->client->setCallback(std::bind(&MqttClient::callback, this, _1, _2, _3));

    int8_t mqttRetryCount = 0;
    for (mqttRetryCount = 0; mqttRetryCount < 10 && !this->client->connected(); mqttRetryCount++) {
        this->serial->println("Connecting to MQTT server...");
        if (this->client->connect(settings->mqttClientId.c_str(), settings->mqttServerUsername.c_str(), settings->mqttServerPassword.c_str() )) {
            this->serial->println("Connected to MQTT server");
            this->client->subscribe(settings->mqttClientTopic.c_str());
        }
        else {
            this->serial->print("Failed with state ");
            this->serial->print(this->client->state());
            delay(2000);
        }
    }

    if (mqttRetryCount >= 10) {
        return false;
    }

    return true;
}

void MqttClient::processState(uint8_t state)
{
    if (this->sentState != state) {
        this->sentState = state;
       
        char payload[41];
        snprintf(payload, 41, "{\"air_flow_state\": %d, \"source\": 0}", state);

        this->serial->println("Sending payload:");
        this->serial->println(payload);
        client->publish(this->mqttTopic, payload);
        this->serial->println(payload);
    }

    client->loop();
}