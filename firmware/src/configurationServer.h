#pragma once
#include "Settings.h"
#include <AsyncElegantOTA.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include "Loggers/ILogger.h"

class ConfigurationServer
{
    private:
        AsyncWebServer* server;
        ILogger* logger;

    public:
        ConfigurationServer(Settings* settings, ILogger* logger);
};