#pragma once

#include <memory>
#include <iostream>

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
    double GetTimeDelta() { return m_TimeDelta / 1000.0; }

private:
    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0; // ms

    // Engine Components
public:
    std::shared_ptr<Renderer> GetRenderer() { return p_Renderer; }
    std::shared_ptr<Camera> GetCamera() { return p_Camera; }

private:
    std::shared_ptr<Renderer> p_Renderer;
    std::shared_ptr<Camera> p_Camera;
};