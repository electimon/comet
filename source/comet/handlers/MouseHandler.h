#pragma once

#include "WindowHandler.h"

class MouseHandler
{
public:
    MouseHandler(const MouseHandler &) = delete;
    static void Create()
    {
        static MouseHandler s_Instance;
    }
    static MouseHandler &Get()
    {
        static MouseHandler s_Instance;
        return s_Instance;
    }

    void SetupCallbacks();

private:
    MouseHandler();
    ~MouseHandler();

    GLFWwindow *p_GLFWwindow;

    void ScrollCallback(double xoffset, double yoffset);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);
};