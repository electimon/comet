#pragma once

#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Engine.h"
#include "handlers/WindowHandler.h"

class Camera
{
public:
    static Camera &GetInstance()
    {
        static Camera s_Instance;
        return s_Instance;
    }

    static void Initialize() { GetInstance().InitializeFunc(); }
    static void Update() { GetInstance().UpdateFunc(); }
    static void CalcViewMatrix() { GetInstance().CalcViewMatrixFunc(); }
    static void CalcProjMatrix() { GetInstance().CalcProjMatrixFunc(); }

    void Move();
    void Rotate();

    static glm::mat4 GetViewMatrix() { return GetInstance().m_ViewMatrix; }
    static glm::mat4 GetProjMatrix() { return GetInstance().m_ProjMatrix; }
    static void SetFOV(float fov) { GetInstance().m_FOV = fov; }
    static void SetYaw(float yaw) { GetInstance().m_Yaw = yaw; }
    static void SetPitch(float pitch) { GetInstance().m_Pitch = pitch; }
    static void SetRoll(float roll) { GetInstance().m_Roll = roll; }
    static glm::vec3 GetPosition() {return GetInstance().m_CameraPosition;}

private:
    Camera() {}
    Camera(Camera const &);
    void operator=(Camera const &);

    void InitializeFunc();
    void UpdateFunc();
    void CalcViewMatrixFunc();
    void CalcProjMatrixFunc();

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
    glm::vec3 POSITIVE_X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 POSITIVE_Y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 POSITIVE_Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 WORLD_ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
};