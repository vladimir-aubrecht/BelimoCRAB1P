#include "AggregateLogger.h"
#include "time.h"

AggregateLogger::AggregateLogger(ILogger* logger1, ILogger* logger2)
{
    this->logger1 = logger1;
    this->logger2 = logger2;
}

String AggregateLogger::appendDateTime(String message)
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return "[Unknown time] " + message;
    }
    
    char dateTime[30];
    strftime(dateTime, 30, "%Y-%B-%d %H:%M:%S", &timeinfo);

    return String(dateTime) + " " + message;
}

void AggregateLogger::error(String message)
{
    message = appendDateTime(message);

    this->logger1->error(message);
    this->logger2->error(message);
}

void AggregateLogger::warning(String message)
{
    message = appendDateTime(message);

    this->logger1->warning(message);
    this->logger2->warning(message);
}

void AggregateLogger::debug(String message)
{
    message = appendDateTime(message);

    this->logger1->debug(message);
    this->logger2->debug(message);
}

void AggregateLogger::render(String* outputLog)
{
    this->logger1->render(outputLog);
    this->logger2->render(outputLog);
}