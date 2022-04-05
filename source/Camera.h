#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine.h"
#include "handlers/WindowHandler.h"
#include "handlers/MouseHandler.h"

class Camera
{
public:
    static Camera &GetInstance()
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

    static void SetFOV(float fov) { GetInstance().m_FOV = fov; }
    static void SetYaw(float yaw) { GetInstance().m_Yaw = yaw; }
    static void SetPitch(float pitch) { GetInstance().m_Pitch = pitch; }
    static void SetRoll(float roll) { GetInstance().m_Roll = roll; }
    static void SetPosition(const glm::vec3 &position) { GetInstance().m_Position = position; }

    static glm::mat4 GetViewMatrix() { return GetInstance().m_ViewMatrix; }
    static glm::mat4 GetProjMatrix() { return GetInstance().m_ProjMatrix; }
    static glm::vec3 GetForwardVector() { return GetInstance().m_ForwardVector; }
    static glm::vec3 GetDirection() { return GetInstance().m_Direction; }
    static glm::vec3 GetPosition() { return GetInstance().m_Position; }

private:
    Camera() {}
    Camera(Camera const &);
    void operator=(Camera const &) {}

    float m_FOV = 0.0f;
    float m_Aspect = 0.0f;
    float m_Near = 0.0f;
    float m_Far = 0.0f;
    float m_Sensitivity = 0.0f;
    float m_Yaw = 0.0f;
    float m_Pitch = 0.0f;
    float m_Roll = 0.0f;
    float m_MovementSensitivity = 0.0f;
    float m_RotationSensitivity = 0.0f;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Direction = {0.0f, 0.0f, 0.0f};

    glm::vec3 m_RightVector = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_UpVector = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_ForwardVector = {0.0f, 0.0f, 0.0f};

    glm::mat4 m_ViewMatrix = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    glm::mat4 m_ProjMatrix = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    /* World Variables */
    glm::vec3 POSITIVE_X = {1.0f, 0.0f, 0.0f};
    glm::vec3 POSITIVE_Y = {0.0f, 1.0f, 0.0f};
    glm::vec3 POSITIVE_Z = {0.0f, 0.0f, 1.0f};
    glm::vec3 WORLD_ORIGIN = {0.0f, 0.0f, 0.0f};
};