#include "AggregateLogger.h"

AggregateLogger::AggregateLogger(ILogger* logger1, ILogger* logger2)
{
    this->logger1 = logger1;
    this->logger2 = logger2;
}

void AggregateLogger::error(String message)
{
    this->logger1->error(message);
    this->logger2->error(message);
}

void AggregateLogger::warning(String message)
{
    this->logger1->warning(message);
    this->logger2->warning(message);
}

void AggregateLogger::debug(String message)
{
    this->logger1->debug(message);
    this->logger2->debug(message);
}

void AggregateLogger::render(String* outputLog)
{
    this->logger1->render(outputLog);
    this->logger2->render(outputLog);
}