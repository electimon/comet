#pragma once

#include <glfw/glfw3.h>

class EventHandler
{
public:
    inline static auto &Instance()
    {
        static EventHandler instance;
        return instance;
    }

    static void Initialize();
    static void PollEvents();

private:
    EventHandler() {}
    EventHandler(EventHandler const &);
    void operator=(EventHandler const &);
};