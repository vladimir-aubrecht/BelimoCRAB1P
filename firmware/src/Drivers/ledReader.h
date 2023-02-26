#pragma once
#include <Arduino.h>

class LedReader
{
    private:
        uint8_t stateCounter = 0;

    public:
        LedReader(uint8_t lowPin, uint8_t comfPin, uint8_t maxPin);

        uint8_t getDistanceFromState(uint8_t newState);
        uint8_t readState();
        void setState(uint8_t state);    // Temporary hack till hw will support reading ...
};