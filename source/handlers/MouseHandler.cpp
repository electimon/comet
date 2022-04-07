#include "MouseHandler.h"

void MouseHandler::SetupCallbacks() {
  glfwSetWindowUserPointer(glfwGetCurrentContext(),
                           &MouseHandler::GetInstance());

  auto ScrollCallbackWrapper = [](GLFWwindow *window, double xoffset,
                                  double yoffset) {
    static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))
        ->ScrollCallback(xoffset, yoffset);
  };
  auto MouseButtonCallbackWrapper = [](GLFWwindow *window, int button,
                                       int action, int mods) {
    static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))
        ->MouseButtonCallback(button, action, mods);
  };
  auto CursorPosCallbackWrapper = [](GLFWwindow *window, double xpos,
                                     double ypos) {
    static_cast<MouseHandler *>(glfwGetWindowUserPointer(window))
        ->CursorPosCallback(xpos, ypos);
  };

  glfwSetScrollCallback(glfwGetCurrentContext(), ScrollCallbackWrapper);
  glfwSetMouseButtonCallback(glfwGetCurrentContext(),
                             MouseButtonCallbackWrapper);
  glfwSetCursorPosCallback(glfwGetCurrentContext(), CursorPosCallbackWrapper);
}

void MouseHandler::ScrollCallback(double xoffset, double yoffset) {}

void MouseHandler::MouseButtonCallback(int button, int action, int mods) {
  // Captures cursor if not currently captured, reguardless of mouse button
  if (action == GLFW_PRESS && !m_CursorCaptured) {
    if (glfwRawMouseMotionSupported()) {
      glfwSetInputMode(WindowHandler::GetInstance().GetGLFWWindow(),
                       GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    CaptureCursor();

    // Need to center cursor before cursor position callback is run
    // Prevents a possibly large xpos/ypos when entering the window
    glfwSetCursorPos(WindowHandler::GetInstance().GetGLFWWindow(), 0.0, 0.0);
  }
}

void MouseHandler::CursorPosCallback(double xpos, double ypos) {
  // Only tracks cursor movement if the cursor is captured
  if (m_CursorCaptured) {
    // Adding up all movement since the last frame was rendered
    m_MovementSinceLastFrame[0] += xpos;
    m_MovementSinceLastFrame[1] -= ypos;

    // Setting cursor back so next callback is relative from center again
    glfwSetCursorPos(WindowHandler::GetInstance().GetGLFWWindow(), 0.0, 0.0);
  } else {
    // No movement if not tracking cursor
    m_MovementSinceLastFrame = {0.0, 0.0};
  }
}