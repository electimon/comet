#pragma once

#include "Engine.h"

class EventHandler
{
public:
    EventHandler(Engine *engine);
    ~EventHandler();

    void PollEvents();

private:
    Engine *p_Engine;
};