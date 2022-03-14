#include "ErrorHandler.h"

ErrorHandler::ErrorHandler(Engine *engine)
    : p_Engine(engine)
{
}

ErrorHandler::~ErrorHandler()
{
}

void ErrorHandler::SetupCallbacks()
{
}

void ErrorHandler::ErrorCallback(int error_code, const char* description)
{
}