#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Engine.h"
#include "../handlers/WindowHandler.h"
#include "../handlers/MouseHandler.h"

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
    void operator=(Camera const &);

    void InitializeFunc();
    void CalcViewMatrixFunc();
    void CalcProjMatrixFunc();

    float m_FOV, m_Aspect, m_Near, m_Far, m_Sensitivity;
    float m_Yaw, m_Pitch, m_Roll;
    float m_MovementSensitivity, m_RotationSensitivity;

    glm::vec3 m_Position;
    glm::vec3 m_Direction;

    glm::vec3 m_RightVector;
    glm::vec3 m_UpVector;
    glm::vec3 m_ForwardVector;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjMatrix;

    /* World Variables */
    glm::vec3 POSITIVE_X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 POSITIVE_Y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 POSITIVE_Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 WORLD_ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
};