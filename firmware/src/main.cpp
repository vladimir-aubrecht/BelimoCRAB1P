#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

uint16_t publishing_delay = 5000;
uint8_t lowPin = 34;
uint8_t comfPin = 35;
uint8_t highPin = 32;
uint8_t switchPin = 33;

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";
const int mqtt_port = 1883;
const char* mqtt_username = "your_MQTT_username_here";
const char* mqtt_password = "your_MQTT_password_here";
const char* mqtt_topic = "your_MQTT_topic_here";

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t readState() {
    uint16_t lowVoltage = analogRead(lowPin);
    uint16_t comfVoltage = analogRead(comfPin);
    uint16_t highVoltage = analogRead(highPin);

    uint8_t selectedState = 0;  // all leds are off

    if (lowVoltage > 1600) {
        selectedState = 1;
    } else if (comfVoltage > 1600) {
        selectedState = 2;
    } else if (highVoltage > 1600) {
        selectedState = 3;
    }

    return selectedState;
}

void setState(uint8_t state) {
    if (state == 0 || state > 3) {
        return;
    }
    
    uint8_t currentState = readState();

    if (state == currentState) {
        return;
    }

    uint8_t steps = 0;

    if (state > currentState) {
        steps = state - currentState;
    } else {
        steps = 3 - currentState + state; 
    }

    for (uint8_t i = 0; i < steps; i++) {
        digitalWrite(switchPin, LOW);
        delay(100);
        digitalWrite(switchPin, HIGH);
        delay(100);
    }

    digitalWrite(switchPin, LOW);
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message content: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    char* message = (char*) payload;

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    
    if (error) {
        Serial.print("Failed to parse JSON data: ");
        Serial.println(error.c_str());
        return;
    }
    
    uint8_t selectedState = doc["air_flow_state"];

    setState(selectedState);
}

void setup() {
    Serial.begin(115200);
    pinMode(lowPin, INPUT_PULLDOWN);
    pinMode(comfPin, INPUT_PULLDOWN);
    pinMode(highPin, INPUT_PULLDOWN);
    pinMode(switchPin, OUTPUT);


    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT server...");
        if (client.connect("BelimoCRAB1P", mqtt_username, mqtt_password )) {
            Serial.println("Connected to MQTT server");
            client.subscribe(mqtt_topic);
        }
        else {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void loop() {
    uint8_t selectedState = readState();

    char payload[21];
    snprintf(payload, 21, "{\"air_flow_state\": %d}", selectedState);
    
    Serial.println("Sending payload:");
    Serial.println(payload);

    client.publish(mqtt_topic, payload);
    Serial.println(payload);
    client.loop();
    delay(publishing_delay);
}