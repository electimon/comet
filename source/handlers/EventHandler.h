#pragma once

#include <GLFW/glfw3.h>

class EventHandler
{
public:
    static EventHandler &GetInstance()
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