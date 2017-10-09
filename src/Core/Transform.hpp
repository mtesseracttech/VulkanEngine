//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_TRANSFORM_HPP
#define VULKANENGINE_TRANSFORM_HPP

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <exception>

class Transform
{
public:
    Transform(const glm::mat4 &m_matrix);
    Transform();
    glm::vec3 GetPosition();
    glm::quat GetRotation();
    glm::vec3 GetForward();
    void SetPosition(const glm::vec3& p_position);
    void SetRotation(const glm::quat& p_rotation);
    void SetRotation(const glm::mat3& p_rotation);
    glm::vec3 GetRight();
    glm::vec3 GetUp();
    void SetForward(const glm::vec3& p_forward, const glm::vec3& p_up);
private:
    glm::mat4 m_matrix;
    glm::quat m_cachedRotation;
    void UpdateCache();
    bool m_isDirty;
};


#endif //VULKANENGINE_TRANSFORM_HPP
