#pragma once

#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "handlers/WindowHandler.h"
#include "Camera.h"

class DebugInfo
{
public:
    static DebugInfo &GetInstance()
    {
        static DebugInfo s_Instance;
        return s_Instance;
    }

    static void Initialize();
    static void DrawFrame();

private:
    DebugInfo() {}
    DebugInfo(DebugInfo const &);
    void operator=(DebugInfo const &);
};