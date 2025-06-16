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
#include <functional>
using namespace std::placeholders;

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
uint8_t lastState = -1;

// choose your NTP server(s)
const char* ntpServer = "pool.ntp.org";
// offset in seconds from UTC (e.g. CET = +1h = 3600)
const long  gmtOffset_sec    = 3600;
// daylight-saving offset in seconds (CET→CEST = +1h)
const int   daylightOffset_sec = 3600;

void configureTime()
{
    int retryCount = 10;
    // configure time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    logger->debug("Waiting for time");
    // option A: block until time is set
    struct tm timeinfo;
    while(!getLocalTime(&timeinfo))
    {
        if (retryCount <= 0)
        {
            break;
        }

        Serial.print(".");
        delay(500);
        retryCount--;
    }
    logger->debug("\nTime initialized");
}

void initMqttClient()
{
     if (mqttClient->initialise(settings, std::bind(&HomeAssistantClient::receiveStates, haClient, _1, _2, _3)))
    {
        haClient->publishDiscoveryConfig();

        delay(10000);
        recuperation->setState(1);
        delay(1000);
        recuperation->setState(3);

        haClient->publishStates(3);    // Home Assistant needs to receive initial state
    }
}

void setup()
{
    Serial.begin(115200);

    ILogger* serialLogger = new SerialLogger(&Serial);
    ILogger* htmlLogger = new HtmlLogger();
    logger = new AggregateLogger(serialLogger, htmlLogger);

    relay = new Relay(RECUPERATION_SWITCH_PIN, RECUPERATION_SWITCH_DELAY, logger);
    ledReader = new LedReader(RECUPERATION_LED_LOW_PIN, RECUPERATION_LED_COMF_PIN, RECUPERATION_LED_HIGH_PIN, logger);
    recuperation = new Recuperation(ledReader, relay, logger);
    wifiClient = new WiFiClient();
    pubSubClient = new PubSubClient(*wifiClient);
    mqttClient = new MqttClient(logger, pubSubClient);
    
    
    settings = new Settings(WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER_HOST, MQTT_SERVER_PORT, MQTT_SERVER_USERNAME, MQTT_SERVER_PASSWORD, MQTT_CLIENT_ID, MQTT_BASE_TOPIC, MQTT_AVAIBILITY_TOPIC, MQTT_SUBSCRIBE_TOPIC);
    settings->read();

    haClient = new HomeAssistantClient(recuperation, logger, mqttClient, settings);

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

    configureTime();

    configurationServer = new ConfigurationServer(settings, logger);

    logger->debug("Mqtt initializing ...");

    initMqttClient();

    logger->debug("Setup finished.");
}

void loop() {
    if (mqttClient->isConnected())
    {
        uint16_t selectedState = ledReader->readState();
        
        if (selectedState != lastState)
        {
            lastState = selectedState;
            haClient->publishStates(selectedState);
        }
    }
    else
    {
        logger->debug("Mqtt initializing ...");
        initMqttClient();
    }
    
    mqttClient->loop();
    delay(MCU_DEFAULT_LOOP_DELAY);
}