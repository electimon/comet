#pragma once

#include <iostream>

#include <glad/gl.h>
#include <glfw/glfw3.h>

class WindowHandler
{
public:
    static WindowHandler &Instance()
    {
        static WindowHandler s_Instance;
        return s_Instance;
    }

    static void Initialize();
    static void CenterWindow();
    static void SetupCallbacks();

    static bool ShouldWindowClose();
    static GLFWwindow *GetGLFWWindow();

    int CreateWindow();

private:
    WindowHandler() {}
    WindowHandler(WindowHandler const &);
    void operator=(WindowHandler const &) {}

    void WindowSizeCallback(int width, int height);
    void FramebufferSizeCallback(int width, int height);

    GLFWwindow *p_GLFWwindow = nullptr;
};
