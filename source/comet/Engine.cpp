#include "Engine.h"

#include "handlers/MouseHandler.h"
#include "handlers/KeyboardHandler.h"
#include "handlers/WindowHandler.h"
#include "handlers/EventHandler.h"
#include "handlers/ErrorHandler.h"

#include "render/Renderer.h"

Engine::Engine()
{
    p_WindowHandler = std::make_shared<WindowHandler>(this);
    p_EventHandler = std::make_shared<EventHandler>(this);
    p_KeyboardHandler = std::make_shared<KeyboardHandler>(this);
    p_MouseHandler = std::make_shared<MouseHandler>(this);
    p_ErrorHandler = std::make_shared<ErrorHandler>(this);

    p_Renderer = std::make_shared<Renderer>(this);

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

void Engine::Stop()
{
    glfwTerminate();
}

void Engine::MainLoop()
{
    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {
        GetRenderer()->NewFrame();

        // renderer should do things
        GetRenderer()->DrawMeshQueue();

        GetRenderer()->EndFrame();

        GetEventHandler()->PollEvents();
    }

    Stop();
}