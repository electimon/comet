#pragma once

#include <comet.pch>

#include "handlers/Interface.h"
#include "handlers/InterfaceHandler.h"

#include "world/World.h"

class RenderMenu : public Interface
{
  public:
    RenderMenu() { InterfaceHandler::AddInterface(this); }
    ~RenderMenu() {}

    void Draw() override
    {
        ImGui::Begin("Render Menu");

        if (ImGui::Button("Play"))
        {
            MouseHandler::CaptureCursor();
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

        if (ImGui::InputInt("Render Distance", &dummyvar1))
        {
            World::SetRenderDistance(dummyvar1);
        }
        if (ImGui::InputInt("World Seed", &dummyvar2))
        {
            ChunkGenerator::SetSeed(dummyvar2);
        }

        if (ImGui::SliderFloat3("Overlay Color", &dummyfloatvec1[0], -1.0f, 1.0f))
        {
            Renderer::SetOverlayColor(dummyfloatvec1);
        }
        if (ImGui::SliderFloat3("Background Color", &dummyfloatvec2[0], 0.0f, 1.0f))
        {
            Renderer::SetBackgroundColor(dummyfloatvec2);
        }

        if (ImGui::Button("Reset Renderer"))
        {
            Renderer::ResetRenderer();
        }

        ImGui::End();
    }

  private:
    int dummyvar1 = 4;
    int dummyvar2 = 4;
    glm::vec3 dummyfloatvec1 = {0.0f, 0.0f, 0.0f};
    glm::vec3 dummyfloatvec2 = {0.0f, 0.0f, 0.0f};
};