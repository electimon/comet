#pragma once

#include <array>

#include <glfw/glfw3.h>

#include "WindowHandler.h"

class MouseHandler
{
public:
    static MouseHandler &GetInstance()
    {
        static MouseHandler s_Instance;
        return s_Instance;
    }

    static void UpdateStates()
    {
        if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            if (!GetInstance().m_LeftClick && !GetInstance().m_LeftHold)
            {
                GetInstance().m_LeftClick = true;
            }
            else
            {
                GetInstance().m_LeftClick = false;
                GetInstance().m_LeftHold = true;
            }
        }
        else
        {
            GetInstance().m_LeftClick = false;
            GetInstance().m_LeftHold = false;
        }

        if (glfwGetMouseButton(WindowHandler::GetGLFWWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (!GetInstance().m_RightClick && !GetInstance().m_RightHold)
            {
                GetInstance().m_RightClick = true;
            }
            else
            {
                GetInstance().m_RightClick = false;
                GetInstance().m_RightHold = true;
            }
        }
        else
        {
            GetInstance().m_RightClick = false;
            GetInstance().m_RightHold = false;
        }
    }

    static void ResetStates()
    {
        GetInstance().m_LeftClick = false;
        GetInstance().m_LeftHold = false;
        GetInstance().m_RightClick = false;
        GetInstance().m_RightHold = false;
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

    static bool GetLeftClick() { return GetInstance().m_LeftClick; };
    static bool GetLeftHold() { return GetInstance().m_LeftHold; };
    static bool GetRightClick() { return GetInstance().m_RightClick; };
    static bool GetRightHold() { return GetInstance().m_RightHold; };

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
    bool m_LeftClick;
    bool m_LeftHold;
    bool m_RightClick;
    bool m_RightHold;
};