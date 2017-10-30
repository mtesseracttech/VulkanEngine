//
// Created by MTesseracT on 8-10-2017.
//

#include "GameObject.hpp"

GameObject::GameObject(const Transform &p_transform) : m_transform(p_transform)
{

}

GameObject::GameObject() : m_transform()
{

}

const Transform& GameObject::GetTransform() const
{
    return m_transform;
}
