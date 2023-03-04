#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "configuration.h"
#include "Drivers/relay.h"
#include "Drivers/ledReader.h"
#include "recuperation.h"

WiFiClient espClient;
PubSubClient client(espClient);
Relay* relay;
LedReader* ledReader;
Recuperation* recuperation;

uint8_t sentStateCounter = -1;

void callback(char* topic, byte* payload, unsigned int length) {
    char* message = (char*) payload;

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.print("Failed to parse JSON data: ");
        Serial.println(error.c_str());
        return;
    }
    
    uint8_t source = doc["source"];

    if (source == 0) {
        return;
    }

    uint8_t selectedState = doc["air_flow_state"];

    recuperation->setState(selectedState);
}

#ifndef LED_BUILTIN
int LED_BUILTIN = 2;
#endif 

void setup() {
    Serial.begin(115200);
    pinMode (LED_BUILTIN, OUTPUT);

    relay = new Relay(RECUPERATION_SWITCH_PIN, RECUPERATION_SWITCH_DELAY, &Serial);
    ledReader = new LedReader(RECUPERATION_LED_LOW_PIN, RECUPERATION_LED_COMF_PIN, RECUPERATION_LED_HIGH_PIN);
    recuperation = new Recuperation(ledReader, relay, &Serial);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    
    client.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT server...");
        if (client.connect(MQTT_CLIENT_ID, MQTT_SERVER_USERNAME, MQTT_SERVER_PASSWORD )) {
            Serial.println("Connected to MQTT server");
            client.subscribe(MQTT_CLIENT_TOPIC);
        }
        else {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    delay(10000);
    recuperation->setState(1);
    delay(1000);
    recuperation->setState(3);
}

void loop() {
    uint16_t selectedState = ledReader->readState();

    if (sentStateCounter != selectedState) {
        sentStateCounter = selectedState;

        char payload[41];
        snprintf(payload, 41, "{\"air_flow_state\": %d, \"source\": 0}", selectedState);
        
        Serial.println("Sending payload:");
        Serial.println(payload);
        client.publish(MQTT_CLIENT_TOPIC, payload);
        Serial.println(payload);
    }

    client.loop();
    
    delay(MCU_DEFAULT_LOOP_DELAY);
}