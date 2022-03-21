#pragma once

#include <memory>
#include <iostream>

#include "Timer.h"

class Engine
{
public:
    static Engine &GetInstance()
    {
        static Engine s_Instance;
        return s_Instance;
    }

    static void Initialize() { GetInstance().InitializeFunction(); }
    static void Terminate();
    static double GetTimeDelta() { return GetInstance().GetTimeDeltaFunction(); }
    static void MainLoop() { GetInstance().MainLoopFunction(); }

private:
    Engine() {}
    Engine(Engine const &);
    void operator=(Engine const &);

    void InitializeFunction();
    void MainLoopFunction();
    double GetTimeDeltaFunction() { return m_TimeDelta; }

    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0;  // ms
};