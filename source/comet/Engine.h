#pragma once

#include <memory>
#include <iostream>

#include "glad/gl.h"
#include "glfw/glfw3.h"

class MouseHandler;
class KeyboardHandler;
class Renderer;
class WindowHandler;
class EventHandler;

class Engine
{
public:
    Engine();
    ~Engine();

    void Start();
    void Stop();

    std::shared_ptr<MouseHandler> GetMouseHandler() { return p_MouseHandler; }
    std::shared_ptr<KeyboardHandler> GetKeyboardHandler() { return p_KeyboardHandler; }
    std::shared_ptr<Renderer> GetRenderer() { return p_Renderer; }
    std::shared_ptr<WindowHandler> GetWindowHandler() { return p_WindowHandler; }
    std::shared_ptr<EventHandler> GetEventHandler() { return p_EventHandler; }

private:
    void MainLoop();

    std::shared_ptr<MouseHandler> p_MouseHandler;
    std::shared_ptr<KeyboardHandler> p_KeyboardHandler;
    std::shared_ptr<Renderer> p_Renderer;
    std::shared_ptr<WindowHandler> p_WindowHandler;
    std::shared_ptr<EventHandler> p_EventHandler;
};