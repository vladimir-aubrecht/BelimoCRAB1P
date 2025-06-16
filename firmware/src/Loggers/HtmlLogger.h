#pragma once
#include "ILogger.h"
#include "Arduino.h"

class HtmlLogger : public ILogger
{
private:
    String outputString;
    int bufferLength = 6000;

public:
    HtmlLogger();

    virtual void error(String message);
    virtual void warning(String message);
    virtual void debug(String message);

    virtual void render(String* outputLog);
};