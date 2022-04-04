#pragma once

#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"
#include "handlers/EntityHandler.h"

#include "render/Renderer.h"
#include "render/Camera.h"
#include "render/DebugInfoUI.h"
#include "render/Texture.h"
#include "render/TextureMap.h"
#include "render/containers/Shader.h"

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

    static bool ShouldClose() { return GetInstance().m_ShouldClose; }
    static void CloseWindow() { GetInstance().m_ShouldClose = true; }

private:
    Engine() {}
    Engine(Engine const &);
    void operator=(Engine const &);

    bool m_ShouldClose = false;
    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0;  // ms
};