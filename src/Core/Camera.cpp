//
// Created by MTesseracT on 8-10-2017.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera()
{
}

Camera::~Camera()
{

}

void Camera::SetStraightRotation(glm::vec3 p_rotation)
{
    glm::mat4 rotation;
    glm::rotate(rotation, glm::radians(p_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); //rotation around x
    glm::rotate(rotation, glm::radians(p_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //rotation around y
    glm::rotate(rotation, glm::radians(p_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); //rotation around z
    //Still need to apply it
}

void Camera::SetPerspective(glm::mat4 p_perspective)
{
    m_perspective = p_perspective;
}

const glm::mat4 &Camera::GetPerspective()
{
    return m_perspective;
}
