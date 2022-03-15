#pragma once

#include "Engine.h"

class WindowHandler
{
public:
    WindowHandler(Engine *engine);
    ~WindowHandler();

    int CreateWindow();
    void CenterWindow();

    void SetupCallbacks();

private:
    Engine *p_Engine;

    GLFWwindow *p_GLFWwindow;
    int m_WindowWidth;
    int m_WindowHeight;

    void WindowSizeCallback(int width, int height);
    void FramebufferSizeCallback(int width, int height);

public:
    GLFWwindow *GetGLFWWindow() { return p_GLFWwindow; }
    int GetWindowWidth() {return m_WindowWidth;}
    int GetWindowHeight() {return m_WindowHeight;}
};