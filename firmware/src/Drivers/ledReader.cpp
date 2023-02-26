#include "ledReader.h"

LedReader::LedReader(uint8_t lowPin, uint8_t comfPin, uint8_t maxPin)
{
    pinMode(lowPin, INPUT_PULLDOWN);
    pinMode(comfPin, INPUT_PULLDOWN);
    pinMode(maxPin, INPUT_PULLDOWN);
}

uint8_t LedReader::getDistanceFromState(uint8_t newState)
{
    uint8_t currentState = this->readState();

    if (newState == currentState) {
        return 0;
    }

    uint8_t steps = 0;

    if (newState > currentState) {
        steps = newState - currentState;
    } else {
        steps = 3 - currentState + newState; 
    }

    return steps;
}

uint8_t LedReader::readState()
{
    /*uint16_t lowVoltage = analogRead(RECUPERATION_LED_LOW_PIN);
    uint16_t comfVoltage = analogRead(RECUPERATION_LED_COMF_PIN);
    uint16_t highVoltage = analogRead(RECUPERATION_LED_HIGH_PIN);

    uint8_t selectedState = 0;  // all leds are off

    if (lowVoltage > 1600) {
        selectedState = 1;
    } else if (comfVoltage > 1600) {
        selectedState = 2;
    } else if (highVoltage > 1600) {
        selectedState = 3;
    }

    return selectedState;
    */

   return this->stateCounter;
}

void LedReader::setState(uint8_t state)
{
    this->stateCounter = state;
}
