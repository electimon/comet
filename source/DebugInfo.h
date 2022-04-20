#pragma once

#include <comet.pch>

#include "Engine.h"
#include "Camera.h"
#include "handlers/WindowHandler.h"

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