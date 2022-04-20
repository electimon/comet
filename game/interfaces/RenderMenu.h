#pragma once

#include <comet.pch>

#include "handlers/Interface.h"
#include "handlers/InterfaceHandler.h"

class RenderMenu : public Interface
{
  public:
    RenderMenu() { InterfaceHandler::AddInterface(this); }
    ~RenderMenu() {}

    void Draw() override
    {
        ImGui::Begin("Test");
        ImGui::Button("Happy!");
        ImGui::End();
    }

  private:
};