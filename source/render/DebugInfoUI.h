#pragma once

#include "glfw/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

        GetInstance().m_TimeDelta = 0.0;
        GetInstance().m_TimeLast = 0.0;
        GetInstance().m_FPS = 0.0;
    }

    static void Update()
    {
        DebugInfoUI::UpdateTime();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Debug Info Panel");
            ImGui::SetWindowSize(ImVec2(400.0f, 50.0f));
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    static void UpdateTime()
    {
        double currentTime = glfwGetTime();
        GetInstance().m_TimeDelta = currentTime - GetInstance().m_TimeLast;
        GetInstance().m_TimeLast = currentTime;

        GetInstance().m_FPS = 1.0 / GetInstance().m_TimeDelta;
    }

private:
    DebugInfoUI() {}
    DebugInfoUI(DebugInfoUI const &);
    void operator=(DebugInfoUI const &);

    double m_FPS;
    double m_TimeLast;
    double m_TimeDelta;
};