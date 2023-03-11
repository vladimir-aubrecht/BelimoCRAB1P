#include "configurationServer.h"


ConfigurationServer::ConfigurationServer(Settings* settings)
{
    WiFi.softAP(settings->mqttClientId.c_str());
    this->server = new AsyncWebServer(80);

    this->server->on("/", HTTP_GET, [this, &settings](AsyncWebServerRequest *request) {
        request->send(200, "text/html", this->generateIndexPage(settings));
    });

    server->on("/credentials", HTTP_POST, [&settings](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Credentials updated.");
        
        const String wifiUsername = request->getParam("wifi_username")->value();
        const String wifiPassword = request->getParam("wifi_password")->value();
        const String mqttHost = request->getParam("mqtt_host")->value();
        const String mqttPort = request->getParam("mqtt_port")->value();
        const String mqttUsername = request->getParam("mqtt_username")->value();
        const String mqttPassword = request->getParam("mqtt_password")->value();
        const String mqttId = request->getParam("mqtt_id")->value();
        const String mqttTopic = request->getParam("mqtt_topic")->value();

        settings->wifiUsername = wifiUsername;
        settings->wifiPassword = wifiPassword;
        settings->mqttServerHost = mqttHost;
        settings->mqttServerPort = (uint16_t)atoi(mqttPort.c_str());
        settings->mqttServerUsername = mqttUsername;
        settings->mqttServerPassword = mqttPassword;
        settings->mqttClientId = mqttId;
        settings->mqttClientTopic = mqttTopic;
        settings->store();
        
        delay(1000);

        ESP.restart();
    });

    this->server->begin();
}