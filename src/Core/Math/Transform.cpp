//
// Created by MTesseracT on 12-2-2018.
//

#include <Core/Math/MathConstants.hpp>
#include <Utility/Logger.hpp>
#include "Transform.hpp"

Transform::Transform() : m_position(glm::vec3(0.0f)), m_rotation(glm::quat()), m_modelMatrix(glm::mat4(1.0f)){}

Transform::Transform(glm::vec3 p_position, glm::quat p_rotation) : m_position(p_position), m_rotation(p_rotation), m_modelMatrix(glm::mat4(1.0f)){}

void Transform::UpdateMatrix()
{
    glm::mat4 rotation = glm::toMat4(m_rotation);
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);

    m_modelMatrix = rotation * transform;
}

void Transform::SetPosition(glm::vec3 p_position)
{
    m_moved = true;
    m_position = m_position;
}

void Transform::SetRotation(glm::quat p_rotation)
{
    m_moved = true;
    m_rotation = p_rotation;
}

void Transform::SetRotation(glm::vec3 p_euler)
{
    m_moved = true;
    m_rotation = glm::quat(p_euler);
}

const glm::vec3 Transform::Position()
{
    return m_position;
}

const glm::quat Transform::Rotation()
{
    return m_rotation;
}

const glm::mat4 Transform::ModelMatrix()
{
    if(m_moved) UpdateMatrix();
    return m_modelMatrix;
}

const glm::vec3 Transform::Forward()
{
    return m_rotation * MathConstants::Dirs::FWD;
}

const glm::vec3 Transform::Right()
{
    return m_rotation * MathConstants::Dirs::RHT;
}

const glm::vec3 Transform::Up()
{
    return m_rotation * MathConstants::Dirs::UP;
}

std::string Transform::GetString()
{
    std::stringstream output;

    glm::vec3 eulerAngles = glm::eulerAngles(m_rotation);

    output << "Local Position: " << m_position << std::endl;
    output << "Local Rotation: " << eulerAngles << std::endl;

    Logger::Log(output.str(), LogType::LogInfo);
}

const glm::mat4 Transform::WorldMatrix()
{
    return m_parent ? ModelMatrix() * m_parent->WorldMatrix() : ModelMatrix();
}

const glm::quat Transform::WorldRotation()
{
    return m_parent ? Rotation() * m_parent->Rotation() : Rotation();
}

const glm::vec3 Transform::WorldPosition()
{
    return m_parent ? glm::vec3(WorldMatrix()[3]) : Position(); //Because position might changed based on parent rotation
}

uint32_t Transform::GetChildCount()
{
    return static_cast<uint32_t>(m_children.size());
}

const Transform* Transform::GetChildAt(uint32_t p_index)
{
    return p_index < GetChildCount() ? m_children[p_index] : nullptr;
}