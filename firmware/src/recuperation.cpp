#include "Recuperation.h"

Recuperation::Recuperation(LedReader* ledReader, Relay* relay, ILogger* logger)
{
    this->ledReader = ledReader;
    this->relay = relay;
    this->logger = logger;
}

void Recuperation::setState(uint8_t state) {
    if (state == 0 || state > 3) {
        return;
    }

    while (this->ledReader->readState() != state)
    {
        this->logger->debug("Requested state: " + String(state));
        this->relay->toggle();
    }

    this->logger->debug("LED and MCU states are in sync.");
}