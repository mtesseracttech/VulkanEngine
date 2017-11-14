//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_CAMERA_HPP
#define VULKANENGINE_CAMERA_HPP


#include "GameObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera
{
private:
    glm::vec3 m_rotation;
    glm::vec3 m_position;
    glm::mat4 m_perspective;
    glm::mat4 m_view;
    float     m_zoom;

    void UpdateMatrix();
public:
    Camera(){};
    virtual ~Camera(){};
    void SetPosition(glm::vec3 p_position);
    void SetRotation(glm::vec3 p_rotation);
    const glm::vec3 GetForward();
    const glm::mat4 GetPerspectiveMat();
    const glm::mat4 GetViewMat();

    const glm::vec3 GetPosition();
    const glm::vec3 GetRotation();

    void SetPerspective(float p_fovY, float p_aspectRatio, float p_nearClipPlane, float p_farClipPlane);
};


#endif //VULKANENGINE_CAMERA_HPP
