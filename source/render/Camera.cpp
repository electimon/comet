#include "Camera.h"

#include "handlers/MouseHandler.h"

void Camera::InitializeFunc()
{
    m_CameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_CameraForward = glm::vec3(0.0f, 0.0f, -1.0f);

    m_FOV = glm::radians(85.0f);
    m_Aspect = 16.0f / 9.0f;
    m_Near = 0.1f;
    m_Far = 1000.0f;

    m_Yaw = glm::radians(0.0f);
    m_Pitch = glm::radians(0.0f);
    m_Roll = glm::radians(0.0f);

    m_MovementSensitivity = 10.0f;
    m_RotationSensitivity = 1.5f;

    CalcViewMatrixFunc();
    CalcProjMatrixFunc();
}

void Camera::CalcViewMatrixFunc()
{
    m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraForward, POSITIVE_Y);
}

void Camera::CalcProjMatrixFunc()
{
    m_ProjMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
}

void Camera::Move()
{
    float magnitude = m_MovementSensitivity * Engine::GetTimeDelta();
    glm::vec3 movementDirection = {0.0f, 0.0f, 0.0f};

    // Used so when walking forward vertical movement doesn't occur.
    glm::vec3 cameraFowardXZ = {m_CameraForward.x, 0.0f, m_CameraForward.z};

    // Sprinting
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        magnitude *= 20;
    }

    // Basic movement processing
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
    {
        movementDirection += cameraFowardXZ;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
    {
        movementDirection -= cameraFowardXZ;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
    {
        movementDirection -= m_CameraRight;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
    {
        movementDirection += m_CameraRight;
    }

    // Fixes diagonal directed movement to not be faster than along an axis.
    // Only happens when holding two buttons that are off axis from each other.
    if (movementDirection.x != 0.0f || movementDirection.y != 0.0f)
    {
        movementDirection = glm::normalize(movementDirection);
    }

    // Still perform up/down movements after normalization.
    // Don't care about limiting speed along verticals.
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        movementDirection += POSITIVE_Y;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        movementDirection -= POSITIVE_Y;
    }

    m_CameraPosition += movementDirection * magnitude;
}

void Camera::Rotate()
{
    // This no longer requires a delta time variable.
    // The delta x and delta y variables from the mouse handler are an
    // accumulation of movement over each frame, and this function is
    // run each frame as well, so no need to rely on a dt.
    m_Yaw += (MouseHandler::GetDeltaX() * m_RotationSensitivity) / 300.0;
    m_Pitch += (MouseHandler::GetDeltaY() * m_RotationSensitivity) / 300.0;

    // Keep yaw angle from getting to imprecise
    if (m_Yaw > glm::radians(360.0f))
    {
        m_Yaw -= glm::radians(360.0f);
    }
    else if (m_Yaw < glm::radians(-360.0f))
    {
        m_Yaw += glm::radians(360.0f);
    }

    // Keep pitch angle from rolling over
    if (m_Pitch > glm::radians(89.999f))
    {
        m_Pitch = glm::radians(89.999f);
    }
    else if (m_Pitch < glm::radians(-89.999f))
    {
        m_Pitch = glm::radians(-89.999f);
    }

    m_CameraDirection.x = glm::cos(m_Yaw) * glm::cos(m_Pitch);
    m_CameraDirection.y = glm::sin(m_Pitch);
    m_CameraDirection.z = glm::sin(m_Yaw) * glm::cos(m_Pitch);

    m_CameraForward = glm::normalize(m_CameraDirection);
    m_CameraRight = glm::cross(m_CameraForward, POSITIVE_Y);
}



void Camera::UpdateFunc()
{
    Move();
    Rotate();

    CalcViewMatrix();

    // Does not need to be done unless fov changes?
    // CalcProjMatrix();
}