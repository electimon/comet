#include "Engine.h"

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"

#include "render/Renderer.h"
#include "render/Camera.h"

void Engine::StartFunc()
{
    m_TimeDelta = 0.0;

    WindowHandler::GetInstance();
    WindowHandler::GetInstance().CreateWindow();

    EventHandler::GetInstance();
    KeyboardHandler::GetInstance();
    MouseHandler::GetInstance();
    ErrorHandler::GetInstance();
    Renderer::GetInstance();

    Camera::GetInstance();
    Camera::Initialize();

    WindowHandler::SetupCallbacks();
    KeyboardHandler::SetupCallbacks();
    MouseHandler::SetupCallbacks();
    ErrorHandler::SetupCallbacks();
}

void Engine::Terminate()
{
    glfwTerminate();
}

void Engine::MainLoopFunc()
{
    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        // Clears color and depth buffers
        Renderer::NewFrame();


        // Update camera views for inputs
        Camera::Update();
        // Reset accumulated movement
        MouseHandler::ResetMovement();


        // Drawing the mesh render queue
        Renderer::DrawMeshQueue();


        // Swaps buffers to display new drawn frame
        Renderer::SwapBuffers();
        // Poll events for next frame
        EventHandler::PollEvents();



        m_TimeDelta = glfwGetTime() - m_TimeLast;
        m_TimeLast = glfwGetTime();
    }

    Terminate();
}