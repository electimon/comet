#pragma once

#include <comet.pch>
#include <glad/gl.h>

class WindowHandler
{
public:
    inline static auto &Instance()
    {
        static WindowHandler instance;
        return instance;
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
