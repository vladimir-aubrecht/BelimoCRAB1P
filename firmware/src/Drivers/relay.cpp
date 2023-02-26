#include "relay.h"

Relay::Relay(uint8_t relayPin, uint16_t delayInMs, HardwareSerial* serial)
{
    this->relayPin = relayPin;
    this->delayInMs = delayInMs;
    this->serial = serial;

    pinMode(relayPin, OUTPUT);
}

void Relay::setRelay(bool enable)
{
    
    if (enable) {
        this->serial->println("Enabling reley");
        digitalWrite(this->relayPin, LOW);
    }
    else {
        this->serial->println("Disabling reley");
        digitalWrite(this->relayPin, HIGH);
    }

    if (this->delayInMs > 0) {
        this->serial->println("Waiting after reley switch");
        delay(this->delayInMs);
    }
}

void Relay::switchOnByCount(uint8_t count)
{
    this->serial->print("Number of steps: ");
    this->serial->println(count);

    for (uint8_t i = 0; i < count; i++) {
        this->setRelay(false);
        this->setRelay(true);
    }

    this->setRelay(false);
}