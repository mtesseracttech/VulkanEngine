//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_CAMERA_HPP
#define VULKANENGINE_CAMERA_HPP


#include "GameObject.hpp"

class Camera : GameObject
{
private:
    glm::mat4 m_perspective;
public:
    Camera();
    virtual ~Camera();
    void SetStraightRotation(glm::vec3 p_rotation);
    void SetPerspective(glm::mat4 p_perspective);
    const glm::mat4& GetPerspective();
};


#endif //VULKANENGINE_CAMERA_HPP
