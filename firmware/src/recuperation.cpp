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

    this->logger->debug("Requested state: ");
    this->logger->debug(String(state));

    while (this->ledReader->readState() != state)
    {
        this->relay->toggle();
    }

    this->logger->debug("State set.");
}