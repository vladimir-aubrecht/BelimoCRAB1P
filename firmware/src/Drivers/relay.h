#pragma once
#include "Arduino.h"


class Relay
{
    private:
        uint8_t relayPin;
        uint16_t delayInMs;
        HardwareSerial* serial;

    public:
        Relay(uint8_t relayPin, uint16_t delayInMS, HardwareSerial* serial);
        void setRelay(bool enable);
        void switchOnByCount(uint8_t count);
};