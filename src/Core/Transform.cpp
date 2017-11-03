//
// Created by MTesseracT on 8-10-2017.
//


#include "Transform.hpp"

Transform::Transform(const glm::mat4 &m_matrix) : m_matrix(m_matrix)
{}

Transform::Transform() : m_matrix(glm::mat4())
{}

glm::vec3 Transform::GetPosition()
{
    return m_matrix[3];
}

glm::quat Transform::GetRotation()
{
    if (m_isDirty) UpdateCache();
    return m_cachedRotation;
}

glm::vec3 Transform::GetRight()
{
    return m_matrix[0];
}

glm::vec3 Transform::GetUp()
{
    return m_matrix[1];
}

glm::vec3 Transform::GetForward()
{
    return m_matrix[2];
}

void Transform::UpdateCache()
{
    m_cachedRotation = glm::quat_cast(m_matrix);
}

void Transform::SetForward(const glm::vec3& p_forward, const glm::vec3& p_up = glm::vec3(0, 1, 0))
{
    glm::vec3 z = glm::normalize(p_forward);
    glm::vec3 x = glm::cross(glm::normalize(p_up), z);
    glm::vec3 y = glm::cross(z, x);

    m_matrix[0] = glm::vec4(x, m_matrix[0][3]);
    m_matrix[1] = glm::vec4(y, m_matrix[1][3]);
    m_matrix[2] = glm::vec4(z, m_matrix[2][3]);

    m_isDirty = true;
}

void Transform::SetPosition(const glm::vec3&p_position)
{
    m_matrix[3] = glm::vec4(p_position, 1);
}

void Transform::SetRotation(const glm::quat& p_rotation)
{
    SetRotation(glm::mat3_cast(p_rotation));
}

void Transform::SetRotation(const glm::mat3& p_rotation)
{
    m_matrix[0] = glm::vec4(p_rotation[0], m_matrix[0][3]);
    m_matrix[1] = glm::vec4(p_rotation[1], m_matrix[1][3]);
    m_matrix[2] = glm::vec4(p_rotation[2], m_matrix[2][3]);
}

const glm::mat4& Transform::GetBaseMatrix() const
{
    return m_matrix;
}
