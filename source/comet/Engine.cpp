#include "Engine.h"

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"

#include "render/Renderer.h"
#include "render/Camera.h"

Engine::Engine()
{
    std::cout << "Engine created." << std::endl;
    m_TimeDelta = 0.0;

    WindowHandler::Create();
    WindowHandler::Get().CreateWindow();

    EventHandler::Create();
    KeyboardHandler::Create();
    MouseHandler::Create();
    ErrorHandler::Create();
    Renderer::Create();
    Camera::Create();
}

Engine::~Engine()
{
}

void Engine::Start()
{
    WindowHandler::SetupCallbacks();
    KeyboardHandler::SetupCallbacks();
    MouseHandler::SetupCallbacks();
    ErrorHandler::SetupCallbacks();
}

void Engine::Terminate()
{
    glfwTerminate();
}

void Engine::MainLoop()
{
    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        // Clears color and depth buffers
        Renderer::NewFrame();

        // Update camera views for inputs
        Camera::Update();

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