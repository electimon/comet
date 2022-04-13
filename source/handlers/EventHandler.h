#pragma once

#include <glfw/glfw3.h>

class EventHandler
{
public:
    static EventHandler &Instance()
    {
        static EventHandler s_Instance;
        return s_Instance;
    }

    static void Initialize();
    static void PollEvents();

private:
    EventHandler() {}
    EventHandler(EventHandler const &);
    void operator=(EventHandler const &);
};