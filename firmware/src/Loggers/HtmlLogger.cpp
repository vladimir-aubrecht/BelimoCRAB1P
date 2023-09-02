#include "HtmlLogger.h"

HtmlLogger::HtmlLogger()
{
    
}

void HtmlLogger::error(String message)
{
    int index = this->outputString.length() - this->bufferLength;
    if (index < 0)
    {
        index = 0;
    }

    this->outputString = this->outputString.substring(index) + "[Error] " + message + "<br />";
}

void HtmlLogger::warning(String message)
{
        int index = this->outputString.length() - this->bufferLength;
    if (index < 0)
    {
        index = 0;
    }

    this->outputString = this->outputString.substring(index) + "[Warning] " + message + "<br />";
}

void HtmlLogger::debug(String message)
{
    int index = this->outputString.length() - this->bufferLength;
    if (index < 0)
    {
        index = 0;
    }

    this->outputString = this->outputString.substring(index) + "[Debug] " + message + "<br />";
}

void HtmlLogger::render(String* outputLog)
{
    (*outputLog) = this->outputString;
}