#pragma once
#include <Arduino.h>
#include "Loggers/ILogger.h"

class LedReader
{
    private:
        uint8_t lowPin;
        uint8_t comfPin;
        uint8_t highPin;
        ILogger* logger;

    public:
        LedReader(uint8_t lowPin, uint8_t comfPin, uint8_t maxPin, ILogger* logger);

        uint16_t readState();
};