//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_CAMERA_HPP
#define VULKANENGINE_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "GameObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

enum CameraType{
    FirstPerson,
    OrbCam
};


class Camera : GameObject
{
private:
    glm::vec2 m_oldMousePos;

    glm::vec3 m_euler;

    glm::quat m_rotQuat;

    glm::vec3 m_position;
    glm::mat4 m_perspective;
    glm::mat4 m_view;

    CameraType m_cameraType = CameraType::FirstPerson;

    float m_cameraSpeed = 1;
    float m_rotationSpeed = 10.0f;

    void UpdateMatrix();
public:
    void SetPosition(glm::vec3 p_position);
    void SetRotation(glm::vec3 p_rotation);
    const glm::vec3 GetForward();
    const glm::vec3 GetRight();

    const glm::mat4 GetPerspectiveMat();
    const glm::mat4 GetViewMat();

    const glm::vec3 GetPosition();
    const glm::vec3 GetRotation();

    void Update();

    void SetPerspective(float p_fovY, float p_aspectRatio, float p_nearClipPlane, float p_farClipPlane);

    void SetCameraType(CameraType p_type);

    void Rotate(glm::vec3 p_rotation);

    const glm::vec3 GetUp();
};


#endif //VULKANENGINE_CAMERA_HPP
