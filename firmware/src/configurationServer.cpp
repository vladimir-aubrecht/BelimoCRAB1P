#include "configurationServer.h"

String ConfigurationServer::generateIndexPage(Settings* settings)
{
    String indexPage = "<html>"
        "<body>"
            "<form method='POST' action='/credentials'>"
                "<table>"
                    "<tr>"
                        "<td>Wifi SSID:</td>"
                        "<td><input type='textbox' name='wifi_ssid' value='" + settings->wifiSSID + "'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Wifi password:</td>"
                        "<td><input type='password' name='wifi_password'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Mqtt host:</td>"
                        "<td><input type='textbox' name='mqtt_host' value='" + settings->mqttServerHost + "'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Mqtt port:</td>"
                        "<td><input type='textbox' name='mqtt_port' value='" + settings->mqttServerPort + "'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Mqtt username:</td>"
                        "<td><input type='textbox' name='mqtt_username' value='" + settings->mqttServerUsername + "'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Mqtt password:</td>"
                        "<td><input type='password' name='mqtt_password'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Mqtt id:</td>"
                        "<td><input type='textbox' name='mqtt_id' value='" + settings->mqttClientId + "'></td>"
                    "</tr>"
                    "<tr>"
                        "<td>Mqtt topic:</td>"
                        "<td><input type='textbox' name='mqtt_topic' value='" + settings->mqttClientTopic + "'></td>"
                    "</tr>"
                "</table>"
                "<input type='submit' value='Update'>"
            "</form>"
        "</body>"
    "</html>";

    return indexPage;
}

ConfigurationServer::ConfigurationServer(Settings* settings, HardwareSerial* serial)
{
    String indexPage = this->generateIndexPage(settings);

    serial->println("Starting WIFI access point ....");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(settings->mqttClientId.c_str());

    serial->print("AP Created with IP: ");
    serial->println(WiFi.softAPIP());

    this->server = new AsyncWebServer(80);

    this->server->on("/", HTTP_GET, [indexPage](AsyncWebServerRequest *request) {
        request->send(200, "text/html", indexPage);
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
        
        delay(1000);

        ESP.restart();
    });

    this->server->begin();
}