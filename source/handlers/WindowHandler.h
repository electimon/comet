#pragma once

#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class WindowHandler
{
public:
    static WindowHandler &GetInstance()
    {
        static WindowHandler s_Instance;
        return s_Instance;
    }

    static void CenterWindow();
    static void SetupCallbacks();

    static bool ShouldWindowClose() { return glfwWindowShouldClose(GetInstance().p_GLFWwindow); }

    int CreateWindow();
    static GLFWwindow *GetGLFWWindow() { return GetInstance().p_GLFWwindow; }

private:
    WindowHandler() {}
    WindowHandler(WindowHandler const &);
    void operator=(WindowHandler const &);

    void WindowSizeCallback(int width, int height);
    void FramebufferSizeCallback(int width, int height);

    GLFWwindow *p_GLFWwindow;
};
