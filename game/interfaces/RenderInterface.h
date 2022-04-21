#pragma once

#include <comet.pch>

#include "handlers/Interface.h"
#include "handlers/InterfaceHandler.h"

#include "world/World.h"

class RenderInterface : public Interface
{
  public:
    RenderInterface() { InterfaceHandler::AddInterface(this); }
    ~RenderInterface() {}

    void Draw() override
    {
        ImGui::Begin("Render Menu");

        if (ImGui::Button("Play"))
        {
            MouseHandler::CaptureCursor();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Renderer"))
        {
            Renderer::ResetRenderer();
            World::Finalize();
            Renderer::SetResetting(false);

            World::Initialize();
            World::InitializeThread();
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit"))
        {
            Engine::SetShouldClose(true);
        }

        ImGui::Text("Render Modes: ");
        ImGui::SameLine();
        if (ImGui::Button("Wireframe"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        ImGui::SameLine();

        if (ImGui::Button("Filled"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (ImGui::InputInt("Render Distance", &m_RenderDistance))
        {
            World::SetRenderDistance(m_RenderDistance);
        }
        if (ImGui::InputInt("World Seed", &m_WorldSeed))
        {
            ChunkGenerator::SetSeed(m_WorldSeed);
        }

        if (ImGui::SliderFloat3("Overlay Color", &m_OverlayColor[0], -1.0f, 1.0f))
        {
            Renderer::SetOverlayColor(m_OverlayColor);
        }
        if (ImGui::SliderFloat3("Background Color", &m_BackgroundColor[0], 0.0f, 1.0f))
        {
            Renderer::SetBackgroundColor(m_BackgroundColor);
        }

        ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);

        ImGui::End();
    }

  private:
    int m_RenderDistance = 4;
    int m_WorldSeed = 4;
    glm::vec3 m_OverlayColor = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_BackgroundColor = {0.0f, 0.0f, 0.0f};
};