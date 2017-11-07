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

void Camera::SetStraightRotation(const glm::vec3 p_rotation)
{
    m_rotation = p_rotation;
    UpdateMatrix();
}

void Camera::SetPosition(const glm::vec3 p_position)
{
    m_transform.GetPosition();
    UpdateMatrix();
}

void Camera::UpdateMatrix()
{
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_transform.GetPosition());

    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m_transform.BaseMatrix() = rotationMatrix;
}

void Camera::SetPerspective(float p_fovY, float p_aspectRatio, float p_nearClipPlane, float p_farClipPlane)
{
    m_perspective = glm::perspective(glm::radians(p_fovY), p_aspectRatio, p_nearClipPlane, p_farClipPlane);
}

const glm::mat4 &Camera::GetPerspective()
{
    return m_perspective;
}
