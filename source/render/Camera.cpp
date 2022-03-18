#include "Camera.h"

void Camera::InitializeFunc()
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

    m_MovementSensitivity = 5000.0f;
    m_RotationSensitivity = 5000.0f;

    CalcViewMatrixFunc();
    CalcProjMatrixFunc();
}

void Camera::CalcViewMatrixFunc()
{
    m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraForward, WORLD_Y);
}

void Camera::CalcProjMatrixFunc()
{
    m_ProjMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
}

void Camera::Rotate(double deltaX, double deltaY)
{
    m_Yaw += float(deltaX) * m_Sensitivity * float(Engine::GetTimeDelta());
    m_Pitch += float(deltaY) * m_Sensitivity * float(Engine::GetTimeDelta());

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

void Camera::Move()
{
    double magnitude = m_MovementSensitivity * Engine::GetTimeDelta();

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
    {
        m_CameraPosition += glm::vec3(0.0f, 0.0f, -magnitude);
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
    {
        m_CameraPosition += glm::vec3(0.0f, 0.0f, magnitude);
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
    {
        m_CameraPosition += glm::vec3(-magnitude, 0.0f, 0.0f);
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
    {
        m_CameraPosition += glm::vec3(magnitude, 0.0f, 0.0f);
    }

    CalcViewMatrix();
}

void Camera::UpdateFunc()
{
    Move();
    // Rotate();
}