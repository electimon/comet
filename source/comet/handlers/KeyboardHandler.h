#pragma once

#include "Engine.h"

class KeyboardHandler
{
public:
    KeyboardHandler(Engine *engine);
    ~KeyboardHandler();

    void SetupCallbacks();

private:
    Engine *p_Engine;

    void KeyCallback(int key, int scancode, int action, int mods);
};