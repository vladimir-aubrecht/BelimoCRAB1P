#pragma once
#include "settings.h"
#include <ESPAsyncWebServer.h>


class ConfigurationServer
{
    private:
        AsyncWebServer* server;

        String generateIndexPage(Settings* settings)
        {
            String indexPage = "<html>"
                "<body>"
                    "<form method='POST' action='/credentials'>"
                        "<table>"
                            "<tr>"
                                "<td>Wifi username:</td>"
                                "<td><input type='textbox' name='wifi_username' value='" + settings->wifiUsername + "'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Wifi password:</td>"
                                "<td><input type='password' name='wifi_password'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Mqtt host:</td>"
                                "<td><input type='textbox' name='mqtt_host value='" + settings->mqttServerHost + "'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Mqtt port:</td>"
                                "<td><input type='textbox' name='mqtt_port value='" + settings->mqttServerPort + "'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Mqtt username:</td>"
                                "<td><input type='textbox' name='mqtt_username value='" + settings->mqttServerUsername + "'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Mqtt password:</td>"
                                "<td><input type='password' name='mqtt_password'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Mqtt id:</td>"
                                "<td><input type='textbox' name='mqtt_id value='" + settings->mqttClientId + "'></td>"
                            "</tr>"
                            "<tr>"
                                "<td>Mqtt topic:</td>"
                                "<td><input type='textbox' name='mqtt_topic value='" + settings->mqttClientTopic + "'></td>"
                            "</tr>"
                        "</table>"
                        "<input type='submit' value='Update'>"
                    "</form>"
                "</body>"
            "</html>";

            return indexPage;
        }

    public:
        ConfigurationServer(Settings* settings);
};