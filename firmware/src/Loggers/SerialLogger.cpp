#include "SerialLogger.h"

SerialLogger::SerialLogger(HardwareSerial* serial)
{
    this->serial = serial;
}

void SerialLogger::error(String message)
{
    this->serial->println(message);
}

void SerialLogger::warning(String message)
{
    this->serial->println(message);
}

void SerialLogger::debug(String message)
{
    this->serial->println(message);
}

void SerialLogger::render(String* outputLog)
{

}