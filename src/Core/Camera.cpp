//
// Created by MTesseracT on 8-10-2017.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Camera.hpp"

void Camera::SetRotation(glm::vec3 p_rotation)
{
    m_rotation = p_rotation;
    UpdateMatrix();
}

void Camera::SetPosition(const glm::vec3 p_position)
{
    m_position = p_position;
    UpdateMatrix();
}

void Camera::SetZoom(float p_zoom)
{
    m_zoom = p_zoom;
}

void Camera::UpdateMatrix()
{
    glm::mat4 rotMat = glm::mat4(1.0f);
    glm::mat4 posMat = glm::translate(glm::mat4(1.0f), m_position);

    rotMat = glm::rotate(rotMat, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotMat = glm::rotate(rotMat, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotMat = glm::rotate(rotMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m_view = posMat * rotMat;
}

void Camera::SetPerspective(float p_fovY, float p_aspectRatio, float p_nearClipPlane, float p_farClipPlane)
{
    m_perspective = glm::perspective(glm::radians(p_fovY), p_aspectRatio, p_nearClipPlane, p_farClipPlane);
}

const glm::mat4 Camera::GetPerspectiveMat()
{
    return m_perspective;
}

const glm::mat4 Camera::GetViewMat()
{
    return m_view;
}

const glm::vec3 Camera::GetForward()
{
    return glm::vec3(m_perspective[2]);
}

const glm::vec3 Camera::GetPosition()
{
    return m_position;
}

const glm::vec3 Camera::GetRotation()
{
    return m_rotation;
}
