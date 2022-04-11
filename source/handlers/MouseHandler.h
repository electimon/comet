#pragma once

#include <array>

#include <glfw/glfw3.h>

#include "WindowHandler.h"

class MouseHandler
{
public:
    static MouseHandler &Get()
    {
        static MouseHandler s_Instance;
        return s_Instance;
    }

    static void UpdateStates()
    {
        if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(),
                               GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            if (!Get().m_LeftClick && !Get().m_LeftHold)
            {
                Get().m_LeftClick = true;
            }
            else
            {
                Get().m_LeftClick = false;
                Get().m_LeftHold = true;
            }
        }
        else
        {
            Get().m_LeftClick = false;
            Get().m_LeftHold = false;
        }

        if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(),
                               GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (!Get().m_RightClick && !Get().m_RightHold)
            {
                Get().m_RightClick = true;
            }
            else
            {
                Get().m_RightClick = false;
                Get().m_RightHold = true;
            }
        }
        else
        {
            Get().m_RightClick = false;
            Get().m_RightHold = false;
        }
    }

    static void ResetStates()
    {
        Get().m_LeftClick = false;
        Get().m_LeftHold = false;
        Get().m_RightClick = false;
        Get().m_RightHold = false;
    }

    static void SetupCallbacks();
    static void ResetMovement()
    {
        Get().m_MovementSinceLastFrame = {0.0, 0.0};
    }

    // +x is moving the mouse right
    // +y is moving the mouse forward
    static double GetDeltaX()
    {
        return Get().m_MovementSinceLastFrame[0];
    }
    static double GetDeltaY()
    {
        return Get().m_MovementSinceLastFrame[1];
    }

    static void CaptureCursor()
    {
        glfwSetInputMode(WindowHandler::GetInstance().GetGLFWWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
        Get().m_CursorCaptured = true;
    }
    static void ReleaseCursor()
    {
        glfwSetInputMode(WindowHandler::GetInstance().GetGLFWWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
        Get().m_CursorCaptured = false;
    }

    static bool GetLeftClick() { return Get().m_LeftClick; };
    static bool GetLeftHold() { return Get().m_LeftHold; };
    static bool GetRightClick() { return Get().m_RightClick; };
    static bool GetRightHold() { return Get().m_RightHold; };

private:
    MouseHandler() {}
    MouseHandler(MouseHandler const &);
    void operator=(MouseHandler const &) {}

    GLFWwindow *p_GLFWwindow = nullptr;

    void ScrollCallback(double xoffset, double yoffset);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);

    std::array<double, 2> m_MovementSinceLastFrame = {0.0, 0.0};
    bool m_CursorCaptured = false;
    bool m_LeftClick = false;
    bool m_LeftHold = false;
    bool m_RightClick = false;
    bool m_RightHold = false;
};