#pragma once

#include <iostream>

#include "glad/gl.h"
#include "glfw/glfw3.h"

class WindowHandler
{
public:
    WindowHandler(const WindowHandler &) = delete;
    static void Create()
    {
        static WindowHandler s_Instance;
    }
    static WindowHandler &Get()
    {
        static WindowHandler s_Instance;
        return s_Instance;
    }

    int CreateWindow();
    int GetWindowWidth() { return m_WindowWidth; }
    int GetWindowHeight() { return m_WindowHeight; }
    void CenterWindow();
    void SetupCallbacks();
    GLFWwindow *GetGLFWWindow() { return p_GLFWwindow; }

private:
    WindowHandler();

    int m_WindowWidth;
    int m_WindowHeight;

    void WindowSizeCallback(int width, int height);
    void FramebufferSizeCallback(int width, int height);

    GLFWwindow *p_GLFWwindow;
};
