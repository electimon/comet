// #include <comet.pch>
#pragma once

#include <comet.pch>
#include <glfw/glfw3.h>

struct Timer
{
    Timer();
    Timer(const std::string &scope);
    ~Timer();

private:
    double m_StartTime;
    std::string m_ScopeName;
};