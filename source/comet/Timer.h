#pragma once

#include <iostream>
#include <string>

#include "glfw/glfw3.h"

struct Timer
{
    Timer()
        : m_StartTime(glfwGetTime()),
          m_ScopeName("")
    {
    }
    Timer(std::string scope)
        : m_StartTime(glfwGetTime()),
          m_ScopeName(scope)
    {
        // weird formatting workaround
        m_ScopeName.append(" ");
    }
    ~Timer()
    {
        std::cout << "Scope " << m_ScopeName << "took " << glfwGetTime() - m_StartTime << " seconds." << std::endl;
    }

private:
    double m_StartTime;
    std::string m_ScopeName;
};