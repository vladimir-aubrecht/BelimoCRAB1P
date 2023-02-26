#pragma once
#include <Arduino.h>
#include "Drivers/ledReader.h"
#include "Drivers/relay.h"

class Recuperation
{
    private:
        LedReader* ledReader;
        Relay* relay;
        HardwareSerial* serial;

    public:
        Recuperation(LedReader* ledReader, Relay* relay, HardwareSerial* serial);
        void setState(uint8_t state);
};