//
// Created by MTesseracT on 8-10-2017.
//

#include "Camera.hpp"
#include "Utility/ConsoleGlm.hpp"

//Sets the rotation of the camera and updates the view matrix
void Camera::SetRotation(glm::vec3 p_rotation)
{
    m_rotation = p_rotation;
    UpdateMatrix();
}

//Sets the position of the camera and updates the view matrix
void Camera::SetPosition(const glm::vec3 p_position)
{
    m_position = p_position;
    UpdateMatrix();
}

//Updates the view matrix using the current rotation and position
void Camera::UpdateMatrix()
{
    glm::mat4 posMat = glm::translate(glm::mat4(1.0f), m_position);

    //std::cout << posMat << std::endl;

    glm::mat4 rotMat = glm::mat4(1.0f);
    rotMat = glm::rotate(rotMat, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotMat = glm::rotate(rotMat, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotMat = glm::rotate(rotMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    switch(m_cameraType){

        case FirstPerson: m_view = glm::inverse(rotMat * posMat); break;
        case OrbCam:      m_view = glm::inverse(posMat * rotMat); break;
    }
}

//Allows to set a perspective matrix for the camera, using FoV (in degrees), aspect ratio, and clip plane limits
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
    return glm::vec3(m_view[2]);
}

const glm::vec3 Camera::GetRight()
{
    return glm::vec3(m_view[0]);
}

const glm::vec3 Camera::GetPosition()
{
    return m_position;
}

const glm::vec3 Camera::GetRotation()
{
    return m_rotation;
}

void Camera::SetCameraType(CameraType p_type)
{
    m_cameraType = p_type;
    UpdateMatrix();
}
