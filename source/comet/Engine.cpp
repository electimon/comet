#include "Engine.h"

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"
#include "handlers/EntityHandler.h"

#include "TextureMap.h"
#include "Renderer.h"
#include "Camera.h"
#include "DebugInfoUI.h"

void Engine::Initialize()
{
    GetInstance().m_TimeDelta = 0.0;

    WindowHandler::GetInstance();
    WindowHandler::GetInstance().CreateWindow();

    EventHandler::GetInstance();
    KeyboardHandler::GetInstance();
    MouseHandler::GetInstance();
    ErrorHandler::GetInstance();
    EntityHandler::GetInstance();

    TextureMap::GetInstance();
    Renderer::GetInstance();
    Camera::GetInstance();

    Camera::Initialize();
    Renderer::Initialize();
    DebugInfoUI::Initialize();


    WindowHandler::SetupCallbacks();
    KeyboardHandler::SetupCallbacks();
    MouseHandler::SetupCallbacks();
    ErrorHandler::SetupCallbacks();
}

void Engine::Terminate()
{
    glfwTerminate();
}

void Engine::MainThread()
{
    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        // Clears color and depth buffers
        Renderer::NewFrame();

        // Update camera views for inputs
        Camera::Update();
        // Reset accumulated movement
        MouseHandler::ResetMovement();
        // Updates entities with an update function
        EntityHandler::UpdateEntities();

        // ALL DRAWING SHOULD HAPPEN AFTER ANY ENTITY MOVEMENT

        // Drawing the mesh render queue
        Renderer::DrawMeshes();
        // Draw UI after everything else
        DebugInfoUI::Update();

        // Swaps buffers to display new drawn frame
        Renderer::SwapBuffers();
        // Poll events for next frame
        EventHandler::PollEvents();

        GetInstance().m_TimeDelta = glfwGetTime() - GetInstance().m_TimeLast;
        GetInstance().m_TimeLast = glfwGetTime();
    }

    Terminate();
}