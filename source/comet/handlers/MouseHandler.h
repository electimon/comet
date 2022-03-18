#pragma once

#include <array>

#include "glfw/glfw3.h"

#include "WindowHandler.h"

class MouseHandler
{
public:
    static MouseHandler &GetInstance()
    {
        static MouseHandler s_Instance;
        return s_Instance;
    }

    static void SetupCallbacks();
    static void ResetMovement() { GetInstance().m_MovementSinceLastFrame = {0.0, 0.0}; }

    // +x is moving the mouse right
    // +y is moving the mouse forward
    static double GetDeltaX() { return GetInstance().m_MovementSinceLastFrame[0]; }
    static double GetDeltaY() { return GetInstance().m_MovementSinceLastFrame[1]; }

    static void CaptureCursor()
    {
        glfwSetInputMode(WindowHandler::GetInstance().GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GetInstance().m_CursorCaptured = true;
    }
    static void ReleaseCursor()
    {
        glfwSetInputMode(WindowHandler::GetInstance().GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        GetInstance().m_CursorCaptured = false;
    }

private:
    MouseHandler() {}
    MouseHandler(MouseHandler const &);
    void operator=(MouseHandler const &);

    GLFWwindow *p_GLFWwindow;

    void ScrollCallback(double xoffset, double yoffset);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);

    std::array<double, 2> m_MovementSinceLastFrame;
    bool m_CursorCaptured;
};