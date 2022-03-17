#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Engine.h"
#include "handlers/WindowHandler.h"

/*

Coordinate System:
+X: global right (right of screen)
+Y: global up (up from screen)
+Z: global back (out of screen)

*/

class Camera
{
public:
    Camera(Engine *engine)
        : p_Engine(engine)
    {
        m_CameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
        m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);

        m_FOV = glm::radians(85.0f);
        m_Aspect = 16.0f / 9.0f;
        m_Near = 0.1f;
        m_Far = 1000.0f;
        m_Sensitivity = 10.0f;

        m_Yaw = 0.0f;
        m_Pitch = 0.0f;
        m_Roll = 0.0f;

        m_MovementSensitivity = 1000.0f;
        m_RotationSensitivity = 1000.0f;

        CalcViewMatrix();
        CalcProjMatrix();

        p_GLFWwindow = WindowHandler::Get().GetGLFWWindow();
    }

    ~Camera()
    {
    }

    void Update()
    {
        Move();
        // Rotate();
    }

    void Move()
    {
        double magnitude = m_MovementSensitivity * p_Engine->GetTimeDelta();

        if (glfwGetKey(p_GLFWwindow, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_CameraPosition += glm::vec3(0.0f, 0.0f, -magnitude);
        }
        if (glfwGetKey(p_GLFWwindow, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_CameraPosition += glm::vec3(0.0f, 0.0f, magnitude);
        }
        if (glfwGetKey(p_GLFWwindow, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_CameraPosition += glm::vec3(-magnitude, 0.0f, 0.0f);
        }
        if (glfwGetKey(p_GLFWwindow, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_CameraPosition += glm::vec3(magnitude, 0.0f, 0.0f);
        }

        CalcViewMatrix();
    }

    void Rotate(double deltaX, double deltaY)
    {
        m_Yaw += float(deltaX) * m_Sensitivity * float(p_Engine->GetTimeDelta());
        m_Pitch += float(deltaY) * m_Sensitivity * float(p_Engine->GetTimeDelta());

        if (m_Yaw > 180.0f)
        {
            m_Yaw -= 360.0f;
        }
        else if (m_Yaw < -180.0f)
        {
            m_Yaw += 360.0f;
        }

        if (m_Pitch > 89.999f)
        {
            m_Pitch = 89.999f;
        }
        else if (m_Pitch < -89.999f)
        {
            m_Pitch = -89.999f;
        }

        m_CameraDirection.x = glm::cos(glm::radians(m_Pitch)) * glm::cos(glm::radians(m_Yaw));
        m_CameraDirection.y = glm::cos(glm::radians(m_Pitch)) * glm::sin(glm::radians(m_Yaw));
        m_CameraDirection.z = glm::sin(glm::radians(m_Pitch));

        m_CameraForward = glm::normalize(m_CameraDirection);
    }

    void CalcViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraForward, WORLD_Y);
    }

    void CalcProjMatrix()
    {
        m_ProjMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
    }

private:
    Engine *p_Engine;
    GLFWwindow* p_GLFWwindow;

    float m_FOV, m_Aspect, m_Near, m_Far, m_Sensitivity;
    float m_Yaw, m_Pitch, m_Roll;
    float m_MovementSensitivity, m_RotationSensitivity;

    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraDirection;

    glm::vec3 m_CameraRight;
    glm::vec3 m_CameraUp;
    glm::vec3 m_CameraForward;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjMatrix;

    /* World Variables */
    glm::vec3 WORLD_X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 WORLD_Y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 WORLD_Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 WORLD_ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);

public: // Set Functions
    void SetFOV(float fov) { m_FOV = fov; }
    void SetYaw(float yaw) { m_Yaw = yaw; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
    void SetRoll(float roll) { m_Roll = roll; }

    glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
    glm::mat4 GetProjMatrix() { return m_ProjMatrix; }
};