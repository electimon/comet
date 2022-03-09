#pragma once

#include "Engine.h"

class MouseHandler
{
public:
    MouseHandler(Engine *engine);
    ~MouseHandler();

    void SetupCallbacks();

private:
    Engine *p_Engine;
    GLFWwindow *p_GLFWwindow;

    void ScrollCallback(double xoffset, double yoffset);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);
};