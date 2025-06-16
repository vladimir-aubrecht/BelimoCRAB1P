#pragma once
#include "ILogger.h"
#include "Arduino.h"

class AggregateLogger : public ILogger
{
private:
    ILogger* logger1;
    ILogger* logger2;

    String appendDateTime(String message);

public:
    AggregateLogger(ILogger* logger1, ILogger* logger2);

    virtual void error(String message);
    virtual void warning(String message);
    virtual void debug(String message);

    virtual void render(String* outputLog);
};