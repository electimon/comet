#include "Engine.h"

void Engine::Initialize() {
  GetInstance().m_TimeDelta = 0.0;

  WindowHandler::Initialize();

  EventHandler::Get();
  KeyboardHandler::Get();
  MouseHandler::Get();
  ErrorHandler::Get();
  EntityHandler::Get();

  TextureMap::Get();
  Renderer::Get();
  Camera::Get();

  Camera::Initialize();
  Renderer::Initialize();
  DebugInfoUI::Initialize();

  WindowHandler::SetupCallbacks();
  KeyboardHandler::SetupCallbacks();
  MouseHandler::SetupCallbacks();
  ErrorHandler::SetupCallbacks();
}

void Engine::Finalize() {
  // Finalizing systems with threads
  glfwTerminate();
}

void Engine::MainThread() {
  while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
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
    DebugInfoUI::DrawFrame();

    // Swaps buffers to display new drawn frame
    Renderer::SwapBuffers();
    // Poll events for next frame
    EventHandler::PollEvents();

    GetInstance().m_TimeDelta = glfwGetTime() - GetInstance().m_TimeLast;
    GetInstance().m_TimeLast = glfwGetTime();
  }

  Engine::CloseWindow();

  Finalize();
}