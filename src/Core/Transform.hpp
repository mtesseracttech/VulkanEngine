//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_TRANSFORM_HPP
#define VULKANENGINE_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <exception>


class Transform
{
private:
    glm::mat4 m_matrix;
    glm::quat m_cachedRotation;
    bool m_isDirty;

public:
    Transform(const glm::mat4 &m_matrix);
    Transform();

    glm::mat4 & BaseMatrix();

    glm::vec3 GetPosition();
    glm::quat GetRotation();

    void SetPosition(const glm::vec3& p_position);
    void SetRotation(const glm::quat& p_rotation);
    void SetRotation(const glm::mat3& p_rotation);

    glm::vec3 GetForward();
    glm::vec3 GetRight();
    glm::vec3 GetUp();

    void SetForward(const glm::vec3& p_forward, const glm::vec3& p_up);
    void Translate(glm::vec3 p_translation);

private:
    void UpdateCache();
};


#endif //VULKANENGINE_TRANSFORM_HPP
