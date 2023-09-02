#pragma once
#include <Arduino.h>
#include "Drivers/LedReader.h"
#include "Drivers/Relay.h"
#include "Loggers/ILogger.h"

class Recuperation
{
    private:
        LedReader* ledReader;
        Relay* relay;
        ILogger* logger;

    public:
        Recuperation(LedReader* ledReader, Relay* relay, ILogger* logger);
        void setState(uint8_t state);
};