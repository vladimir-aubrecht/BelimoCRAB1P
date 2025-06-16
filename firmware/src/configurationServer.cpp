#include "ConfigurationServer.h"
#include <ArduinoOTA.h>

ConfigurationServer::ConfigurationServer(Settings* settings, ILogger* logger)
{
    this->logger = logger;

    String indexTemplate = "";

    if(SPIFFS.begin(true))
    {
        File file = SPIFFS.open("/index.html");
        if(!file){
            logger->warning("Files in filesystem not found. Upload it.");
        }

        indexTemplate = file.readString();
        
        if (file)
        {
            file.close();
        }
    }
    else
    {
        logger->error("An Error has occurred while mounting SPIFFS");
    }

    if(indexTemplate.length() <= 0){
        logger->warning("HTML template seems broken.");
    }    

    this->server = new AsyncWebServer(80);

    this->server->on("/", HTTP_GET, [indexTemplate, settings, logger](AsyncWebServerRequest *request) {
        String buffer = "";
        logger->render(&buffer);
        
        String indexPage = String(indexTemplate);
        indexPage.replace("{wifiSSID}", settings->wifiSSID);
        indexPage.replace("{wifiPassword}", settings->wifiPassword);
        indexPage.replace("{mqttServerHost}", settings->mqttServerHost);
        indexPage.replace("{mqttServerPort}", String(settings->mqttServerPort));
        indexPage.replace("{mqttServerUsername}", settings->mqttServerUsername);
        indexPage.replace("{mqttServerPassword}", settings->mqttServerPassword);
        indexPage.replace("{mqttClientId}", settings->mqttClientId);
        indexPage.replace("{mqttBaseTopic}", settings->mqttBaseTopic);
        indexPage.replace("{mqttAvaibilityTopic}", settings->mqttAvaibilityTopic);
        indexPage.replace("{mqttSubscribeTopic}", settings->mqttSubscribeTopic);
        
        request->send(200, "text/html", indexPage + buffer);
    });

    server->on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Restarted.");
        ESP.restart();
    });

    server->on("/credentials", HTTP_POST, [settings](AsyncWebServerRequest *request) {
        const String wifiSSID = request->getParam("wifi_ssid", true)->value();
        const String wifiPassword = request->getParam("wifi_password", true)->value();
        const String mqttHost = request->getParam("mqtt_host", true)->value();
        const String mqttPort = request->getParam("mqtt_port", true)->value();
        const String mqttUsername = request->getParam("mqtt_username", true)->value();
        const String mqttPassword = request->getParam("mqtt_password", true)->value();
        const String mqttId = request->getParam("mqtt_id", true)->value();
        const String mqttBaseTopic = request->getParam("mqtt_base_topic", true)->value();
        const String mqttAvaibilityTopic = request->getParam("mqtt_avaibility_topic", true)->value();
        const String mqttSubscribeTopic = request->getParam("mqtt_subscribe_topic", true)->value();

        settings->wifiSSID = wifiSSID;
        settings->wifiPassword = wifiPassword;
        settings->mqttServerHost = mqttHost;
        settings->mqttServerPort = (uint16_t)atoi(mqttPort.c_str());
        settings->mqttServerUsername = mqttUsername;
        settings->mqttServerPassword = mqttPassword;
        settings->mqttClientId = mqttId;
        settings->mqttBaseTopic = mqttBaseTopic;
        settings->mqttAvaibilityTopic = mqttAvaibilityTopic;
        settings->mqttSubscribeTopic = mqttSubscribeTopic;
        settings->store();

        request->send(200, "text/plain", "Credentials updated.");
    });

    AsyncElegantOTA.begin(this->server);
    this->server->begin();
    ArduinoOTA.setTimeout(30000);
    this->logger->debug("Waiting for 60 seconds after startup for potentional flash of firmware (in case something is wrong ...)");
    
    for (int i = 0; i < 10; i++)
    {
        delay(6000);
        this->logger->debug("Waiting...");
    }
}