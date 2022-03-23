#pragma once

#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

#include "Timer.h"

class Engine
{
public:
    static Engine &GetInstance()
    {
        static Engine s_Instance;
        return s_Instance;
    }

    static void Initialize();
    static void Terminate();
    static void MainThread();

    static double GetTimeDelta() { return GetInstance().m_TimeDelta; }

private:
    Engine() {}
    Engine(Engine const &);
    void operator=(Engine const &);

    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0;  // ms
};