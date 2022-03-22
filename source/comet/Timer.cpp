#include "Timer.h"

Timer::Timer()
    : m_StartTime(glfwGetTime()),
      m_ScopeName("")
{
}

Timer::Timer(std::string scope)
    : m_StartTime(glfwGetTime()),
      m_ScopeName(scope)
{
    // weird formatting workaround
    m_ScopeName.append(" ");
}

Timer::~Timer()
{
    std::cout << "Scope " << m_ScopeName << "took " << glfwGetTime() - m_StartTime << " seconds." << std::endl;
}