#include "MouseHandler.h"

MouseHandler::MouseHandler()
{
}

MouseHandler::~MouseHandler()
{
}

void MouseHandler::SetupCallbacks()
{
    glfwSetWindowUserPointer(glfwGetCurrentContext(), &MouseHandler::Get());

    auto ScrollCallbackWrapper = [](GLFWwindow *window, double xoffset, double yoffset)
    {
        static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))->ScrollCallback(xoffset, yoffset);
    };
    auto MouseButtonCallbackWrapper = [](GLFWwindow *window, int button, int action, int mods)
    {
        static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))->MouseButtonCallback(button, action, mods);
    };
    auto CursorPosCallbackWrapper = [](GLFWwindow *window, double xpos, double ypos)
    {
        static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))->CursorPosCallback(xpos, ypos);
    };

    glfwSetScrollCallback(glfwGetCurrentContext(), ScrollCallbackWrapper);
    glfwSetMouseButtonCallback(glfwGetCurrentContext(), MouseButtonCallbackWrapper);
    glfwSetCursorPosCallback(glfwGetCurrentContext(), CursorPosCallbackWrapper);
}

void MouseHandler::ScrollCallback(double xoffset, double yoffset)
{
}

void MouseHandler::MouseButtonCallback(int button, int action, int mods)
{
}

void MouseHandler::CursorPosCallback(double xpos, double ypos)
{
}