//
// Created by MTesseracT on 12-2-2018.
//

#ifndef VULKANENGINE_TRANSFORM_HPP
#define VULKANENGINE_TRANSFORM_HPP


#include <Utility/ConsoleGlm.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <Core/GameObject.hpp>

class Transform
{
private:
    glm::quat m_rotation;
    glm::vec3 m_position;
    glm::mat4 m_modelMatrix;

    bool m_moved = true;

    std::vector<Transform *> m_children;
    Transform *m_parent;

    GameObject *m_gameObject;

    void UpdateMatrix();

public:
    Transform();

    Transform(glm::vec3 p_position, glm::quat p_rotation);

    void SetRotation(glm::quat p_rotation);

    void SetPosition(glm::vec3 p_position);

    void SetRotation(glm::vec3 p_euler);

    const glm::vec3 Position();

    const glm::quat Rotation();

    const glm::mat4 ModelMatrix();

    const glm::mat4 WorldMatrix();

    const glm::quat WorldRotation();

    const glm::vec3 WorldPosition();

    const glm::vec3 Forward();

    const glm::vec3 Right();

    const glm::vec3 Up();

    std::string GetString();

    uint32_t GetChildCount();

    const Transform* GetChildAt(uint32_t p_index);

    std::vector<Transform *>::iterator begin() { return m_children.begin(); }

    std::vector<Transform *>::iterator end() { return m_children.end(); }
};


#endif //VULKANENGINE_TRANSFORM_HPP
