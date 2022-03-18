#pragma once

#include "glfw/glfw3.h"

class MouseHandler
{
public:
    static MouseHandler &GetInstance()
    {
        static MouseHandler s_Instance;
        return s_Instance;
    }

    static void SetupCallbacks();

private:
    MouseHandler() {}
    MouseHandler(MouseHandler const &);
    void operator=(MouseHandler const &);

    GLFWwindow *p_GLFWwindow;

    void ScrollCallback(double xoffset, double yoffset);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);
};