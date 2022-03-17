#pragma once

#include "glfw/glfw3.h"

class EventHandler
{
public:
    EventHandler(const EventHandler &) = delete;
    static void Create()
    {
        static EventHandler s_Instance;
    }
    static EventHandler &Get()
    {
        static EventHandler s_Instance;
        return s_Instance;
    }

    void PollEvents();

private:
    EventHandler();
    ~EventHandler();
};