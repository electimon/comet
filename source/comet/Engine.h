#pragma once

#include <memory>
#include <iostream>

#include "glad/gl.h"
#include "glfw/glfw3.h"

class WindowHandler;
class EventHandler;
class ErrorHandler;
class MouseHandler;
class KeyboardHandler;
class Renderer;
class Camera;

class Engine
{
public:
    Engine();
    ~Engine();

    void Start();
    void Terminate();
    void MainLoop();

    // Returns the time delta from frame to frame in seconds
    double GetTimeDelta() { return m_TimeDelta / 1000; }

private:
    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0; // ms

    // Engine Components
public:
    std::shared_ptr<WindowHandler> GetWindowHandler() { return p_WindowHandler; }
    std::shared_ptr<MouseHandler> GetMouseHandler() { return p_MouseHandler; }
    std::shared_ptr<KeyboardHandler> GetKeyboardHandler() { return p_KeyboardHandler; }
    std::shared_ptr<EventHandler> GetEventHandler() { return p_EventHandler; }
    std::shared_ptr<ErrorHandler> GetErrorHandler() { return p_ErrorHandler; }
    std::shared_ptr<Renderer> GetRenderer() { return p_Renderer; }
    std::shared_ptr<Camera> GetCamera() { return p_Camera; }

private:
    std::shared_ptr<WindowHandler> p_WindowHandler;
    std::shared_ptr<MouseHandler> p_MouseHandler;
    std::shared_ptr<KeyboardHandler> p_KeyboardHandler;
    std::shared_ptr<EventHandler> p_EventHandler;
    std::shared_ptr<ErrorHandler> p_ErrorHandler;
    std::shared_ptr<Renderer> p_Renderer;
    std::shared_ptr<Camera> p_Camera;
};