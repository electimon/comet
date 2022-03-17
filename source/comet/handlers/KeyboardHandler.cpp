#include "KeyboardHandler.h"

KeyboardHandler::KeyboardHandler()
{
    SetupCallbacks();

    glfwSetInputMode(glfwGetCurrentContext(), GLFW_STICKY_KEYS, GLFW_TRUE);
}

KeyboardHandler::~KeyboardHandler()
{
}

void KeyboardHandler::SetupCallbacks()
{
    glfwSetWindowUserPointer(glfwGetCurrentContext(), this);

    auto KeyCallbackWrapper = [](GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        static_cast<KeyboardHandler *>(glfwGetWindowUserPointer(window))->KeyCallback(key, scancode, action, mods);
    };

    glfwSetKeyCallback(glfwGetCurrentContext(), KeyCallbackWrapper);
}

void KeyboardHandler::KeyCallback(int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), 1);
    }
    if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
    {
        WindowHandler::Get().CenterWindow();
    }
    if (key == GLFW_KEY_A)
    {
        std::cout << "A" << std::endl;
    }
}