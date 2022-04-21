#pragma once

#include <comet.pch>
#include <glad/gl.h>

#include "Engine.h"

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

    int CreateWindow();

  private:
    WindowHandler() {}
    WindowHandler(WindowHandler const &);
    void operator=(WindowHandler const &) {}

    void WindowSizeCallback(int width, int height);
    void FramebufferSizeCallback(int width, int height);
    void WindowCloseCallback();

    GLFWwindow *m_GLFWWindow = nullptr;

  public:
    static GLFWwindow *GLFWWindow() { return Instance().m_GLFWWindow; }
};
