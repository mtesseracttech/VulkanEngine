//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_GAMEOBJECT_HPP
#define VULKANENGINE_GAMEOBJECT_HPP


#include <Core/MtVulkanAPI/VulkanModel.hpp>
#include "Transform.hpp"

class GameObject
{
public:
    GameObject();
    GameObject(const Transform &p_transform);

    const Transform& GetTransform() const;
private:
    Transform     m_transform;
};


#endif //VULKANENGINE_GAMEOBJECT_HPP
