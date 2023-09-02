#pragma once
#include "Arduino.h"
#include "Loggers/ILogger.h"

class Relay
{
    private:
        uint8_t relayPin;
        uint16_t delayInMs;
        ILogger* logger;

    public:
        Relay(uint8_t relayPin, uint16_t delayInMS, ILogger* logger);
        void setRelay(bool enable);
        void toggle();
};