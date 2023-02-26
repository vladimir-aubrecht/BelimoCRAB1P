#include "recuperation.h"

Recuperation::Recuperation(LedReader* ledReader, Relay* relay, HardwareSerial* serial)
{
    this->ledReader = ledReader;
    this->relay = relay;
    this->serial = serial;
}

void Recuperation::setState(uint8_t state) {
    if (state == 0 || state > 3) {
        return;
    }

    this->serial->print("Requested state: ");
    this->serial->println(state);
    
    uint8_t steps = this->ledReader->getDistanceFromState(state);

    this->ledReader->setState(state);
    this->relay->switchOnByCount(steps);

    this->serial->println("State set.");
}