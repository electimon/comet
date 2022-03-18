#pragma once

#include "glfw/glfw3.h"
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
    static Camera &GetInstance()
    {
        static Camera s_Instance;
        return s_Instance;
    }

    static void Initialize() { GetInstance().InitializeFunc(); }
    static void Update() { GetInstance().UpdateFunc(); }
    static void CalcViewMatrix() { GetInstance().CalcViewMatrixFunc(); }
    static void CalcProjMatrix() { GetInstance().CalcProjMatrixFunc(); }
    static glm::mat4 GetViewMatrix() { return GetInstance().GetViewMatrixFunc(); }
    static glm::mat4 GetProjMatrix() { return GetInstance().GetProjMatrixFunc(); }

    void Move();
    void Rotate(double deltaX, double deltaY);
    void SetFOV(float fov) { m_FOV = fov; }
    void SetYaw(float yaw) { m_Yaw = yaw; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
    void SetRoll(float roll) { m_Roll = roll; }

private:
    Camera() {}
    Camera(Camera const &);
    void operator=(Camera const &);

    void InitializeFunc();
    void UpdateFunc();
    void CalcViewMatrixFunc();
    void CalcProjMatrixFunc();
    glm::mat4 GetViewMatrixFunc() { return m_ViewMatrix; }
    glm::mat4 GetProjMatrixFunc() { return m_ProjMatrix; }

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
};