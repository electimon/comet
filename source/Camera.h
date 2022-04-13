#pragma once

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine.h"
#include "handlers/MouseHandler.h"
#include "handlers/WindowHandler.h"

class Camera
{
public:
    static Camera &Instance()
    {
        static Camera s_Instance;
        return s_Instance;
    }

    static void Initialize();
    static void Update();
    static void CalcViewMatrix();
    static void CalcProjMatrix();

    static void Move();
    static void Rotate();

    static void SetFOV(float fov) { Instance().m_FOV = fov; }
    static void SetYaw(float yaw) { Instance().m_Yaw = yaw; }
    static void SetPitch(float pitch) { Instance().m_Pitch = pitch; }
    static void SetRoll(float roll) { Instance().m_Roll = roll; }
    static void SetPosition(const glm::vec3 &position) { Instance().m_Position = position; }

    static glm::mat4 GetViewMatrix() { return Instance().m_ViewMatrix; }
    static glm::mat4 GetProjMatrix() { return Instance().m_ProjMatrix; }
    static glm::vec3 GetForwardVector() { return Instance().m_ForwardVector; }
    static glm::vec3 GetDirection() { return Instance().m_Direction; }
    static glm::vec3 GetPosition() { return Instance().m_Position; }

private:
    Camera() {}
    Camera(Camera const &);
    void operator=(Camera const &) {}

    float m_FOV{glm::radians(85.0f)};
    float m_Aspect{16.0f / 9.0f};
    float m_Near{0.1f};
    float m_Far{1000.0f};
    float m_Sensitivity{0.0f};
    float m_Yaw{glm::radians(0.0f)};
    float m_Pitch{glm::radians(0.0f)};
    float m_Roll{glm::radians(0.0f)};
    float m_MovementSensitivity{10.0f};
    float m_RotationSensitivity{1.5f};

    glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
    glm::vec3 m_Direction{0.0f, 0.0f, 0.0f};

    glm::vec3 m_RightVector{1.0f, 0.0f, 0.0f};
    glm::vec3 m_UpVector{0.0f, 1.0f, 0.0f};
    glm::vec3 m_ForwardVector{0.0f, 0.0f, -1.0f};

    glm::mat4 m_ViewMatrix{0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f};
    glm::mat4 m_ProjMatrix{0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f};

    /* World Variables */
    glm::vec3 POSITIVE_X{1.0f, 0.0f, 0.0f};
    glm::vec3 POSITIVE_Y{0.0f, 1.0f, 0.0f};
    glm::vec3 POSITIVE_Z{0.0f, 0.0f, 1.0f};
    glm::vec3 WORLD_ORIGIN{0.0f, 0.0f, 0.0f};
};