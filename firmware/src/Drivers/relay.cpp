#include "Relay.h"

Relay::Relay(uint8_t relayPin, uint16_t delayInMs, ILogger* logger)
{
    this->relayPin = relayPin;
    this->delayInMs = delayInMs;
    this->logger = logger;

    pinMode(relayPin, OUTPUT);
}

void Relay::setRelay(bool enable)
{
    
    if (enable) {
        this->logger->debug("Enabling reley");
        digitalWrite(this->relayPin, LOW);
    }
    else {
        this->logger->debug("Disabling reley");
        digitalWrite(this->relayPin, HIGH);
    }

    if (this->delayInMs > 0) {
        this->logger->debug("Waiting after reley switch");
        delay(this->delayInMs);
    }
}

void Relay::toggle()
{
        this->setRelay(false);
        this->setRelay(true);
        this->setRelay(false);
}