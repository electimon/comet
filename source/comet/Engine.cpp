#include "Engine.h"

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/WindowHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"

#include "render/Renderer.h"
#include "render/Camera.h"

Engine::Engine()
{
    m_TimeDelta = 0.0;

    p_WindowHandler = std::make_shared<WindowHandler>(this);
    p_EventHandler = std::make_shared<EventHandler>(this);
    p_KeyboardHandler = std::make_shared<KeyboardHandler>(this);
    p_MouseHandler = std::make_shared<MouseHandler>(this);
    p_ErrorHandler = std::make_shared<ErrorHandler>(this);
    p_Renderer = std::make_shared<Renderer>(this);
    p_Camera = std::make_shared<Camera>(this);

    GetWindowHandler()->CreateWindow();
}

Engine::~Engine()
{
}

void Engine::Start()
{
    p_WindowHandler->SetupCallbacks();
    p_KeyboardHandler->SetupCallbacks();
    p_MouseHandler->SetupCallbacks();
    p_ErrorHandler->SetupCallbacks();
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
        GetEventHandler()->PollEvents();
    }

    Terminate();
}