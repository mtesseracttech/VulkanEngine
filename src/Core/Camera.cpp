//
// Created by MTesseracT on 8-10-2017.
//

#include <Utility/KeyInput.hpp>
#include <Utility/GameTimer.hpp>
#include <Utility/MouseInput.hpp>
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
    glm::mat4 posMat;
    posMat = glm::translate(glm::mat4(1.0f), m_position);


    switch(m_cameraType){

        case FirstPerson: m_view = m_rotationMatrix * posMat;
        case OrbCam:      m_view = posMat * m_rotationMatrix;
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

void Camera::Rotate(glm::vec3 p_rotation)
{
    m_rotation += p_rotation;
    UpdateMatrix();
}

void Camera::SetCameraType(CameraType p_type)
{
    m_cameraType = p_type;
    UpdateMatrix();
}

void Camera::Update()
{
    auto delta = static_cast<float>(GameTimer::Delta());

    if(m_cameraType == FirstPerson)
    {
        const glm::vec3 up(0.0f, 1.0f, 0.0f);

        float correctedSpeed = delta * m_cameraSpeed;

        if(KeyInput::Pressed(GLFW_KEY_W)) m_position += GetForward() * correctedSpeed;
        if(KeyInput::Pressed(GLFW_KEY_S)) m_position -= GetForward() * correctedSpeed;
        if(KeyInput::Pressed(GLFW_KEY_A)) m_position -= GetRight() * correctedSpeed;
        if(KeyInput::Pressed(GLFW_KEY_D)) m_position += GetRight() * correctedSpeed;

        if(MouseInput::Pressed(GLFW_MOUSE_BUTTON_1))
        {
            glm::vec2 mousePos   = MouseInput::Position();
            glm::vec2 mouseDelta = m_oldMousePos - mousePos;
            float mouseSpeed = m_cameraSpeed * delta;

            glm::mat4 rotationMat(1);
            rotationMat = glm::rotate(rotationMat, glm::radians(mouseDelta.x * mouseSpeed), glm::vec3(0,1,0));
            rotationMat = glm::rotate(rotationMat, glm::radians(mouseDelta.y * mouseSpeed), glm::vec3(1,0,0));

            m_rotationMatrix *= rotationMat;
        }

        m_oldMousePos = MouseInput::Position();

        UpdateMatrix();
    }
}
