#pragma once
#include "ILogger.h"
#include "Arduino.h"

class SerialLogger : public ILogger
{
private:
    HardwareSerial* serial;

public:
    SerialLogger(HardwareSerial* serial);

    virtual void error(String message);
    virtual void warning(String message);
    virtual void debug(String message);

    virtual void render(String* outputLog);
};