#pragma once

#include "Engine.h"

class ErrorHandler
{
public:
    ErrorHandler(Engine *engine);
    ~ErrorHandler();

    void SetupCallbacks();

private:
    Engine *p_Engine;

    void ErrorCallback(int error_code, const char* description);
};