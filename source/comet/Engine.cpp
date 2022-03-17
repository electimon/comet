#include "Engine.h"

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"

#include "render/Renderer.h"
#include "render/Camera.h"

Engine::Engine()
{
    m_TimeDelta = 0.0;

    WindowHandler::Create();
    WindowHandler::Get().CreateWindow();

    EventHandler::Create();
    KeyboardHandler::Create();
    MouseHandler::Create();
    ErrorHandler::Create();

    p_Renderer = std::make_shared<Renderer>(this);
    p_Camera = std::make_shared<Camera>(this);
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
        GetRenderer()->NewFrame();

        // Update camera views for inputs
        GetCamera()->Update();

        // Drawing the mesh render queue
        GetRenderer()->DrawMeshQueue();

        // Swaps buffers to display new drawn frame
        GetRenderer()->SwapBuffers();

        // Poll events for next frame
        EventHandler::Get().PollEvents();


        m_TimeDelta = glfwGetTime() - m_TimeLast;
        m_TimeLast = glfwGetTime();
    }

    Terminate();
}