#pragma once

#include <comet.pch>

#include "WindowHandler.h"

class MouseHandler
{
  public:
    inline static auto &Instance()
    {
        static MouseHandler instance;
        return instance;
    }

    static void Initialize();
    static void UpdateStates();
    static void ResetStates();
    static void SetupCallbacks();
    static void ResetMovement();

    // +x is moving the mouse right
    // +y is moving the mouse forward
    static double GetDeltaX();
    static double GetDeltaY();

    static void CaptureCursor();
    static void ReleaseCursor();

    static bool GetLeftClick();
    static bool GetLeftHold();
    static bool GetRightClick();
    static bool GetRightHold();

  private:
    MouseHandler() {}
    MouseHandler(MouseHandler const &);
    void operator=(MouseHandler const &) {}

    GLFWwindow *p_GLFWwindow = nullptr;

    void ScrollCallback(double xoffset, double yoffset);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);

    std::array<double, 2> m_MovementSinceLastFrame = {0.0, 0.0};
    bool m_CursorCaptured = false;
    bool m_LeftClick = false;
    bool m_LeftHold = false;
    bool m_RightClick = false;
    bool m_RightHold = false;

    double m_ScrollOffset;

  public:
    static double ScrollOffset() { return Instance().m_ScrollOffset; }
    static void SetScrollOffset(double ScrollOffset) { Instance().m_ScrollOffset = ScrollOffset; }
};