#pragma once

#include <comet.pch>

#include "handlers/EntityHandler.h"
#include "handlers/ErrorHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/MouseHandler.h"

#include "Camera.h"
#include "DebugInfo.h"
#include "Renderer.h"
#include "render/Shader.h"
#include "render/Texture.h"
#include "render/TextureMap.h"

#include "Timer.h"

class Engine
{
public:
    inline static auto &Instance()
    {
        static Engine instance;
        return instance;
    }

    static void Initialize();
    static void Finalize();

    static void MainThread();
    static double GetTimeDelta() { return Instance().m_TimeDelta; }
    static bool ShouldClose() { return Instance().m_ShouldClose; }
    static void CloseWindow() { Instance().m_ShouldClose = true; }

private:
    Engine() {}
    Engine(Engine const &);
    void operator=(Engine const &);

    bool m_ShouldClose = false;
    double m_TimeDelta = 0.0; // ms
    double m_TimeLast = 0.0;  // ms
};