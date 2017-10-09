//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_GAMEOBJECT_HPP
#define VULKANENGINE_GAMEOBJECT_HPP


#include "Transform.hpp"

class GameObject
{
public:
    GameObject();
    GameObject(const Transform &p_transform);
private:
    Transform m_transform;
};


#endif //VULKANENGINE_GAMEOBJECT_HPP
