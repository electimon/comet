#include "WindowHandler.h"

WindowHandler::WindowHandler()
{
}

int WindowHandler::CreateWindow()
{
    // Initialize the library
    if (!glfwInit())
    {
        std::cout << "[Error] Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // Create a windowed mode window and its OpenGL context
    p_GLFWwindow = glfwCreateWindow(1, 1, "Comet (OpenGL 4.6)", NULL, NULL);
    if (!p_GLFWwindow)
    {
        std::cout << "[Error] Failed to create OpenGL window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(p_GLFWwindow);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "[Error] Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    glfwSetWindowUserPointer(p_GLFWwindow, this);

    CenterWindow();

    glfwShowWindow(p_GLFWwindow);

    return 0;
}

void WindowHandler::CenterWindow()
{
    int count;
    int monitorX, monitorY;

    GLFWmonitor **monitors = glfwGetMonitors(&count);
    const GLFWvidmode *videoMode = glfwGetVideoMode(monitors[0]);

    m_WindowWidth = int(videoMode->width / 1.5);
    m_WindowHeight = int(m_WindowWidth / 16 * 9);

    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    glfwSetWindowPos(p_GLFWwindow, monitorX + (videoMode->width - m_WindowWidth) / 2, monitorY + (videoMode->height - m_WindowHeight) / 2);
    glfwSetWindowSize(p_GLFWwindow, m_WindowWidth, m_WindowHeight);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);
}

void WindowHandler::SetupCallbacks()
{
    glfwSetWindowUserPointer(glfwGetCurrentContext(), this);

    auto WindowSizeCallbackWrapper = [](GLFWwindow *window, int width, int height)
    {
        static_cast<WindowHandler *>(glfwGetWindowUserPointer(window))->WindowSizeCallback(width, height);
    };
    auto FrameBufferCallbackWrapper = [](GLFWwindow *window, int width, int height)
    {
        static_cast<WindowHandler *>(glfwGetWindowUserPointer(window))->FramebufferSizeCallback(width, height);
    };

    glfwSetWindowSizeCallback(glfwGetCurrentContext(), WindowSizeCallbackWrapper);
    glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), FrameBufferCallbackWrapper);
}

void WindowHandler::WindowSizeCallback(int width, int height)
{
}

void WindowHandler::FramebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
}