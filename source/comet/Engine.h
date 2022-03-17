#pragma once

#include <memory>
#include <iostream>

class Engine
{
public:
    Engine(const Engine &) = delete;
    static void Create()
    {
        static Engine s_Instance;
    }
    static Engine &Get()
    {
        static Engine s_Instance;
        return s_Instance;
    }

    static void Start();
    static void Terminate();
    static double GetTimeDelta() { return Get().GetTimeDeltaFunc(); }

    void MainLoop();

private:
    Engine();
    ~Engine();

    double GetTimeDeltaFunc() { return m_TimeDelta / 1000.0; }

    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0;  // ms
};