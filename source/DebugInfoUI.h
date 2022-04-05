#pragma once

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "handlers/WindowHandler.h"

class DebugInfoUI
{
public:
    static DebugInfoUI &GetInstance()
    {
        static DebugInfoUI s_Instance;
        return s_Instance;
    }

    static void Initialize()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(WindowHandler::GetGLFWWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    static void DrawFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Debug Info Panel");

            ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetWindowSize(ImVec2(400.0f, 50.0f));
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:
    DebugInfoUI() {}
    DebugInfoUI(DebugInfoUI const &);
    void operator=(DebugInfoUI const &);
};