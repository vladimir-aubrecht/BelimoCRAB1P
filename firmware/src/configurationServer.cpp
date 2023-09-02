#include "ConfigurationServer.h"

ConfigurationServer::ConfigurationServer(Settings* settings, ILogger* logger)
{
    if(!SPIFFS.begin(true)){
        logger->error("An Error has occurred while mounting SPIFFS");
        return;
    }

    File file = SPIFFS.open("/index.html");
    if(!file){
        logger->error("Files in filesystem not found. Upload it.");
        return;
    }

    String indexTemplate = file.readString();
    file.close();

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
        indexPage.replace("{mqttClientTopic}", settings->mqttClientTopic);
        
        request->send(200, "text/html", indexPage + buffer);
    });

    server->on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
        ESP.restart();
        request->send(200, "text/plain", "Restarted.");
    });

    server->on("/credentials", HTTP_POST, [settings](AsyncWebServerRequest *request) {
        const String wifiSSID = request->getParam("wifi_ssid", true)->value();
        const String wifiPassword = request->getParam("wifi_password", true)->value();
        const String mqttHost = request->getParam("mqtt_host", true)->value();
        const String mqttPort = request->getParam("mqtt_port", true)->value();
        const String mqttUsername = request->getParam("mqtt_username", true)->value();
        const String mqttPassword = request->getParam("mqtt_password", true)->value();
        const String mqttId = request->getParam("mqtt_id", true)->value();
        const String mqttTopic = request->getParam("mqtt_topic", true)->value();

        settings->wifiSSID = wifiSSID;
        settings->wifiPassword = wifiPassword;
        settings->mqttServerHost = mqttHost;
        settings->mqttServerPort = (uint16_t)atoi(mqttPort.c_str());
        settings->mqttServerUsername = mqttUsername;
        settings->mqttServerPassword = mqttPassword;
        settings->mqttClientId = mqttId;
        settings->mqttClientTopic = mqttTopic;
        settings->store();

        request->send(200, "text/plain", "Credentials updated.");
    });

    AsyncElegantOTA.begin(this->server);
    this->server->begin();
}