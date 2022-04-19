#pragma once

#include <comet.pch>

#include "handlers/WindowHandler.h"
#include "Camera.h"

class DebugInfo
{
public:
    inline static auto &Instance()
    {
        static DebugInfo instance;
        return instance;
    }

    static void Initialize();
    static void DrawFrame();

private:
    DebugInfo() {}
    DebugInfo(DebugInfo const &);
    void operator=(DebugInfo const &);
};