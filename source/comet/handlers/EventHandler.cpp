#include "EventHandler.h"

#include "glfw/glfw3.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::PollEvents()
{
    glfwPollEvents();
}