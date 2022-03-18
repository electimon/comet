#pragma once

#include <memory>
#include <iostream>

class Engine
{
public:
    static Engine &GetInstance()
    {
        static Engine s_Instance;
        return s_Instance;
    }

    static void Start() { GetInstance().StartFunc(); }
    static void Terminate();
    static double GetTimeDelta() { return GetInstance().GetTimeDeltaFunc(); }
    static void MainLoop() { GetInstance().MainLoopFunc(); }

private:
    Engine() {}
    Engine(Engine const &);
    void operator=(Engine const &);

    void StartFunc();
    void MainLoopFunc();
    double GetTimeDeltaFunc() { return m_TimeDelta / 1000.0; }

    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0;  // ms
};