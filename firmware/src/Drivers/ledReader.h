#pragma once
#include <Arduino.h>

class LedReader
{
    private:
        uint8_t lowPin;
        uint8_t comfPin;
        uint8_t highPin;

    public:
        LedReader(uint8_t lowPin, uint8_t comfPin, uint8_t maxPin);

        uint16_t readState();
};