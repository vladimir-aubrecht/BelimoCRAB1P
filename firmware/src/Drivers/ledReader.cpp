#include "LedReader.h"
#include <string>

LedReader::LedReader(uint8_t lowPin, uint8_t comfPin, uint8_t maxPin, ILogger* logger)
{
    this->lowPin = lowPin;
    this->comfPin = comfPin;
    this->highPin = highPin;
    this->logger = logger;

    pinMode(lowPin, INPUT_PULLDOWN);
    pinMode(comfPin, INPUT_PULLDOWN);
    pinMode(maxPin, INPUT_PULLDOWN);
}

uint16_t LedReader::readState()
{
    // enabled LED will have voltage around 0.3 V
    // disabled LEDs will have voltage around 2 V

    uint16_t voltageLow = analogRead(this->lowPin);
    uint16_t voltageComf = analogRead(this->comfPin);
    uint16_t voltageHigh = analogRead(this->highPin);

    uint8_t selectedState = 0;  // all leds are off

    this->logger->debug("LED voltages, low: " + String(voltageLow) + " comf: " + String(voltageComf) + " high: " + String(voltageHigh));

    if (voltageLow < 500) {
        selectedState = 1;
    } else if (voltageComf < 500) {
        selectedState = 2;
    } else if (voltageHigh < 500) {
        selectedState = 3;
    }

    this->logger->debug("Found LED state: " + String(selectedState));

    return selectedState;
}
