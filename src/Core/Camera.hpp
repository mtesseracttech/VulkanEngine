//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_CAMERA_HPP
#define VULKANENGINE_CAMERA_HPP


#include "GameObject.hpp"

class Camera : GameObject
{
private:
    glm::vec3 m_rotation;
    glm::mat4 m_perspective;

    void UpdateMatrix();
public:
    Camera();
    virtual ~Camera();
    void SetPosition(const glm::vec3 p_position);
    void SetStraightRotation(glm::vec3 p_rotation);
    const glm::mat4& GetPerspective();
    void SetPerspective(float p_fovY, float p_aspectRatio, float p_nearClipPlane, float p_farClipPlane);
};


#endif //VULKANENGINE_CAMERA_HPP
