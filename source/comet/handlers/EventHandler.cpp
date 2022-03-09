#include "EventHandler.h"

EventHandler::EventHandler(Engine *engine)
    : p_Engine(engine)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::PollEvents()
{
    glfwPollEvents();
}