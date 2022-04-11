#pragma once

#include <glfw/glfw3.h>

class EventHandler
{
public:
    static EventHandler &Get()
    {
        static EventHandler s_Instance;
        return s_Instance;
    }
    static void PollEvents();

private:
    EventHandler() {}
    EventHandler(EventHandler const &);
    void operator=(EventHandler const &);
};