//
// Created by MTesseracT on 8-10-2017.
//

#include <Utility/KeyInput.hpp>
#include <Utility/GameTimer.hpp>
#include <Utility/MouseInput.hpp>
#include "Camera.hpp"
#include "Utility/ConsoleGlm.hpp"
#include <glm/gtx/quaternion.hpp>

//Sets the rotation of the camera and updates the view matrix
void Camera::SetRotation(glm::vec3 p_rotation)
{
    m_euler = p_rotation;
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
    glm::mat4 rotMat = glm::toMat4(m_rotQuat);

    glm::mat4 posMat = glm::translate(glm::mat4(1.0f), m_position);


    switch(m_cameraType){

        case FirstPerson: m_view = rotMat * posMat;
        case OrbCam:      m_view = posMat * rotMat;
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
    return glm::vec3(-m_view[2]); //Because forward is inverted from OpenGL
}

const glm::vec3 Camera::GetRight()
{
    return glm::vec3(m_view[0]);
}

const glm::vec3 Camera::GetUp()
{
    return glm::vec3(-m_view[1]); //Idem Dito
}

const glm::vec3 Camera::GetPosition()
{
    return m_position;
}

const glm::vec3 Camera::GetRotation()
{
    return m_euler;
}

void Camera::Rotate(glm::vec3 p_rotation)
{
    m_euler += p_rotation;
    m_rotQuat = glm::quat(glm::radians(m_euler));
    UpdateMatrix();
}

void Camera::SetCameraType(CameraType p_type)
{
    m_cameraType = p_type;
    UpdateMatrix();
}

void Camera::Update()
{
    auto dT = static_cast<float>(GameTimer::Delta());

    if(m_cameraType == FirstPerson)
    {
        float adjustedCamSpeed = m_cameraSpeed * dT;

        if(MouseInput::Pressed(MouseButton::Button1))
        {
            glm::vec2 mousePos   = MouseInput::Position();
            glm::vec2 mouseDelta = m_oldMousePos - mousePos;

            glm::vec2 adjustedDelta = mouseDelta * m_rotationSpeed * dT;

            m_euler += glm::vec3(-adjustedDelta.y, adjustedDelta.x, 0);
        }

        m_rotQuat = glm::quat(glm::radians(m_euler));

        if(KeyInput::Pressed(KeyCode::W)) m_position += GetForward() * adjustedCamSpeed;
        if(KeyInput::Pressed(KeyCode::S)) m_position -= GetForward() * adjustedCamSpeed;
        if(KeyInput::Pressed(KeyCode::A)) m_position -= GetRight() * adjustedCamSpeed;
        if(KeyInput::Pressed(KeyCode::D)) m_position += GetRight() * adjustedCamSpeed;
        if(KeyInput::Pressed(KeyCode::Space)) m_position += glm::vec3(0,-1,0) * adjustedCamSpeed; //-1 is used because of Vulkans space system (rhs with Y+ going down)
        if(KeyInput::Pressed(KeyCode::Shift)) m_position -= glm::vec3(0,-1,0) * adjustedCamSpeed;

        m_oldMousePos = MouseInput::Position();

        UpdateMatrix();
    }
}
