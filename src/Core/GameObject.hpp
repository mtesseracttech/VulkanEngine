//
// Created by MTesseracT on 8-10-2017.
//

#ifndef VULKANENGINE_GAMEOBJECT_HPP
#define VULKANENGINE_GAMEOBJECT_HPP

#include <Core/MtVulkanAPI/VulkanRenderable.hpp>
#include <Behaviours/Behaviour.hpp>

class GameObject
{
public:
    //Child management
    void RemoveChild(GameObject * p_child);
    void AddChild(GameObject * p_child);

    uint32_t GetChildCount();
    GameObject * GetChildAtIndex(uint32_t p_index);

    //Parent Management
    virtual void SetParent(GameObject * p_parent);
    GameObject * GetParent();


    virtual void Update();

    void GenerateCommandBufferInfo(vk::CommandBuffer p_drawCommandBuffer, vk::PipelineLayout p_pipelineLayout, uint32_t p_vertexBufferBinding, vk::DeviceSize * p_offset);
protected:
    void RemoveFromChildren(GameObject * p_child);
    void AddToChildren(GameObject * p_child);

    //Descriptor
    std::string              m_name;
    //Geometry related
    glm::mat4                m_transform;
    //TODO: Creating new Transform object

    //Related Objects
    GameObject *             m_parent           = nullptr;
    std::vector<GameObject*> m_children;
    VulkanRenderable *       m_renderComponent  = nullptr;
    Behaviour *              m_behaviour        = nullptr;


};


#endif //VULKANENGINE_GAMEOBJECT_HPP
