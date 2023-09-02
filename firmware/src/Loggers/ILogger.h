#pragma once
#include "ilogger.h"
#include "Arduino.h"

class ILogger
{
public:
    virtual void error(String message) = 0;
    virtual void warning(String message) = 0;
    virtual void debug(String message) = 0;

    virtual void render(String* outputLog) = 0;
};