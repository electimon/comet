#pragma once

#include "WindowHandler.h"

class KeyboardHandler
{
public:
    KeyboardHandler(const KeyboardHandler &) = delete;
    static void Create()
    {
        static KeyboardHandler s_Instance;
    }
    static KeyboardHandler &Get()
    {
        static KeyboardHandler s_Instance;
        return s_Instance;
    }

    void SetupCallbacks();

private:
    KeyboardHandler();
    ~KeyboardHandler();

    void KeyCallback(int key, int scancode, int action, int mods);
};