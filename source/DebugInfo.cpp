#include "DebugInfo.h"

void DebugInfo::Initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(WindowHandler::GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void DebugInfo::DrawFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Debug Info Panel");

        ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetWindowSize(ImVec2(400.0f, 100.0f));

        ImGui::Text("Application average: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        ImGui::Text("Camera position: %.3f, %.3f, %.3f", Camera::Position().x, Camera::Position().y,
                    Camera::Position().z);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
