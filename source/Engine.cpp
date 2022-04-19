#include "Engine.h"

void Engine::Initialize()
{
  Instance().m_TimeDelta = 0.0;

  // Engine Components
  WindowHandler::Initialize();
  EventHandler::Initialize();
  KeyboardHandler::Initialize();
  MouseHandler::Initialize();
  ErrorHandler::Initialize();
  EntityHandler::Initialize();

  // Rendering Components
  TextureMap::Initialize();
  Renderer::Initialize();
  Camera::Initialize();
  DebugInfo::Initialize();
}

void Engine::Finalize()
{
  // Finalizing systems with threads
  glfwTerminate();
}

void Engine::MainThread()
{
  while (!glfwWindowShouldClose(glfwGetCurrentContext()))
  {
    // Clears color and depth buffers
    Renderer::NewFrame();

    // Update all handlers
    MouseHandler::UpdateStates();

    // Update camera views for inputs
    Camera::Update();
    // Reset accumulated movement
    MouseHandler::ResetMovement();

    // Updates entities with an update function
    // Moved to separate thread since it is unreleated to rendering
    EntityHandler::UpdateEntities();

    // ALL DRAWING SHOULD HAPPEN AFTER ANY ENTITY MOVEMENT

    // Drawing the mesh render queue
    Renderer::DrawMeshQueue();
    // Draw UI after everything else
    DebugInfo::DrawFrame();

    // Swaps buffers to display new drawn frame
    Renderer::SwapBuffers();
    // Poll events for next frame
    EventHandler::PollEvents();

    Instance().m_TimeDelta = glfwGetTime() - Instance().m_TimeLast;
    Instance().m_TimeLast = glfwGetTime();
  }

  Engine::SetShouldClose(true);

  Finalize();
}