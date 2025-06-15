#include <Arduino.h>
#include <WiFi.h>
#include "Configuration.h"
#include "Loggers/SerialLogger.h"
#include "Loggers/HtmlLogger.h"
#include "Loggers/AggregateLogger.h"
#include "Drivers/Relay.h"
#include "Drivers/LedReader.h"
#include "Recuperation.h"
#include "Clients/MqttClient.h"
#include "ConfigurationServer.h"
#include "Settings.h"
#include "Clients/HomeAssistantClient.h"

Relay* relay;
LedReader* ledReader;
Recuperation* recuperation;
MqttClient* mqttClient;
ConfigurationServer* configurationServer = NULL;
ILogger* logger = NULL;
Settings* settings = NULL;
PubSubClient* pubSubClient = NULL;
WiFiClient* wifiClient = NULL;
HomeAssistantClient* haClient;

void setup() {
    Serial.begin(115200);

    ILogger* serialLogger = new SerialLogger(&Serial);
    ILogger* htmlLogger = new HtmlLogger();
    logger = new AggregateLogger(serialLogger, htmlLogger);

    relay = new Relay(RECUPERATION_SWITCH_PIN, RECUPERATION_SWITCH_DELAY, logger);
    ledReader = new LedReader(RECUPERATION_LED_LOW_PIN, RECUPERATION_LED_COMF_PIN, RECUPERATION_LED_HIGH_PIN, logger);
    recuperation = new Recuperation(ledReader, relay, logger);
    wifiClient = new WiFiClient();
    pubSubClient = new PubSubClient(*wifiClient);
    haClient = new HomeAssistantClient(recuperation, logger, pubSubClient);
    mqttClient = new MqttClient(recuperation, logger, pubSubClient, haClient);
    
    settings = new Settings(WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER_HOST, MQTT_SERVER_PORT, MQTT_SERVER_USERNAME, MQTT_SERVER_PASSWORD, MQTT_CLIENT_ID, MQTT_CLIENT_TOPIC);
    settings->read();

    WiFi.begin(settings->wifiSSID.c_str(), settings->wifiPassword.c_str());
    int8_t wifiRetryCount = 0;
    for (wifiRetryCount = 0; wifiRetryCount < 10 && WiFi.status() != WL_CONNECTED; wifiRetryCount++) {
        delay(1000);
        logger->debug("Connecting to WiFi...");
    }

    if (wifiRetryCount >= 10) {
        logger->warning("Giving up connecting to wifi.");
        logger->debug("Starting WIFI access point ....");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(settings->mqttClientId.c_str());

        logger->debug("AP Created with IP: ");
        logger->debug(WiFi.softAPIP().toString());
    }

    logger->debug("Connected to WiFi");


    configurationServer = new ConfigurationServer(settings, logger);

    logger->debug("Mqtt initializing ...");

    if (mqttClient->initialise(settings))
    {
        delay(10000);
        recuperation->setState(1);
        delay(1000);
        recuperation->setState(3);

        mqttClient->processState(3);    // Home Assistant needs to receive initial state
    }

    logger->debug("Setup finished.");
}

void loop() {
    if (mqttClient->isConnected()) {
        uint16_t selectedState = ledReader->readState();
        mqttClient->processState(selectedState);
    }
    else
    {
        logger->debug("Mqtt initializing ...");
        mqttClient->initialise(settings);
    }

    delay(MCU_DEFAULT_LOOP_DELAY);
}