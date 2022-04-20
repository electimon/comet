#include "Camera.h"

void Camera::Initialize()
{
    Camera &camera = Instance();

    CalcViewMatrix();
    CalcProjMatrix();
}

void Camera::CalcViewMatrix()
{
    Camera &camera = Instance();
    camera.m_ViewMatrix = glm::lookAt(camera.m_Position, camera.m_Position + camera.m_ForwardVector, camera.POSITIVE_Y);
}

void Camera::CalcProjMatrix()
{
    Camera &camera = Instance();
    camera.m_ProjMatrix = glm::perspective(camera.m_FOV, camera.m_Aspect, camera.m_Near, camera.m_Far);
}

void Camera::Move()
{
    Camera &camera = Instance();

    float magnitude = camera.m_MovementSpeed * Engine::TimeDelta();
    glm::vec3 movementDirection = {0.0f, 0.0f, 0.0f};

    // Used so when walking forward vertical movement doesn't occur.
    glm::vec3 cameraFowardXZ = {camera.m_ForwardVector.x, 0.0f, camera.m_ForwardVector.z};

    // Sprinting
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        magnitude *= 10;
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
        movementDirection -= camera.m_RightVector;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
    {
        movementDirection += camera.m_RightVector;
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
        movementDirection += camera.POSITIVE_Y;
    }
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        movementDirection -= camera.POSITIVE_Y;
    }

    camera.m_Position += movementDirection * magnitude;
}

void Camera::Rotate()
{
    Camera &camera = Instance();

    // This no longer requires a delta time variable.
    // The delta x and delta y variables from the mouse handler are an
    // accumulation of movement over each frame, and this function is
    // run each frame as well, so no need to rely on a dt.
    camera.m_Yaw += (MouseHandler::GetDeltaX() * camera.m_RotationSpeed) / 300.0;
    camera.m_Pitch += (MouseHandler::GetDeltaY() * camera.m_RotationSpeed) / 300.0;

    // Keep yaw angle from getting to imprecise
    if (camera.m_Yaw > glm::radians(360.0f))
    {
        camera.m_Yaw -= glm::radians(360.0f);
    }
    else if (camera.m_Yaw < glm::radians(-360.0f))
    {
        camera.m_Yaw += glm::radians(360.0f);
    }

    // Keep pitch angle from going too far over
    if (camera.m_Pitch > glm::radians(89.0f))
    {
        camera.m_Pitch = glm::radians(89.0f);
    }
    else if (camera.m_Pitch < glm::radians(-89.0f))
    {
        camera.m_Pitch = glm::radians(-89.0f);
    }

    camera.m_Direction.x = glm::cos(camera.m_Yaw) * glm::cos(camera.m_Pitch);
    camera.m_Direction.y = glm::sin(camera.m_Pitch);
    camera.m_Direction.z = glm::sin(camera.m_Yaw) * glm::cos(camera.m_Pitch);

    camera.m_ForwardVector = glm::normalize(camera.m_Direction);
    camera.m_RightVector = glm::cross(camera.m_ForwardVector, camera.POSITIVE_Y);
}

void Camera::Update()
{
    Move();
    Rotate();

    CalcViewMatrix();

    // Does not need to be done unless fov changes?
    // CalcProjMatrix();
}