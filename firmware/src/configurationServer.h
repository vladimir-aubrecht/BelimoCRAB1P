#pragma once
#include "settings.h"
#include <ESPAsyncWebServer.h>


class ConfigurationServer
{
    private:
        AsyncWebServer* server;
        String generateIndexPage(Settings* settings);

    public:
        ConfigurationServer(Settings* settings, HardwareSerial* serial);
};