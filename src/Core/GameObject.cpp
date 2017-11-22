//
// Created by MTesseracT on 8-10-2017.
//

#include "GameObject.hpp"

void GameObject::Update()
{
    if(m_behaviour) m_behaviour->Update();

    for (auto i = static_cast<int>(m_children.size() - 1); i >= 0; --i)
    {
        m_children[i]->Update();
    }
}

void GameObject::RemoveChild(GameObject *p_child)
{
    p_child->SetParent(nullptr);
}

void GameObject::AddChild(GameObject *p_child)
{
    p_child->SetParent(this);
}

uint32_t GameObject::GetChildCount()
{
    return static_cast<uint32_t>(m_children.size());
}

GameObject *GameObject::GetChildAtIndex(uint32_t p_index)
{
    if(p_index < m_children.size()) return m_children[p_index];
}

void GameObject::SetParent(GameObject *p_parent)
{
    if(m_parent)
    {
        m_parent->RemoveFromChildren(this);
        m_parent = nullptr;
    }

    if(p_parent)
    {
        m_parent = p_parent;
        m_parent->AddToChildren(this);
    }
}

GameObject *GameObject::GetParent()
{
    return m_parent;
}

void GameObject::RemoveFromChildren(GameObject *p_child)
{
    for (auto child = m_children.begin(); child != m_children.end(); ++child)
    {
        if(*child == p_child)
        {
            (*child)->m_parent = nullptr;
            m_children.erase(child);
            return;
        }
    }
}

void GameObject::AddToChildren(GameObject *p_child)
{
    p_child->m_parent = this;
    m_children.push_back(p_child);
}

void GameObject::GenerateCommandBufferInfo(vk::CommandBuffer p_drawCommandBuffer, vk::PipelineLayout p_pipelineLayout,
                                           uint32_t p_vertexBufferBinding, vk::DeviceSize *p_offset)
{
    if(m_renderComponent) m_renderComponent->BindDrawData(p_drawCommandBuffer, p_pipelineLayout, p_vertexBufferBinding, p_offset);

    for (auto child = m_children.begin(); child != m_children.end(); ++child)
    {
        (*child)->GenerateCommandBufferInfo(p_drawCommandBuffer, p_pipelineLayout, p_vertexBufferBinding, p_offset);
    }
}
