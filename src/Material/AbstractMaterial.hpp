//
// Created by MTesseracT on 2-11-2017.
//

#ifndef VULKANENGINE_ABSTRACTMATERIAL_HPP
#define VULKANENGINE_ABSTRACTMATERIAL_HPP


#include <vulkan/vulkan.hpp>
#include <Core/MtVulkanAPI/VulkanTexture.hpp>
#include <Core/GameObject.hpp>

class AbstractMaterial
{
protected:
    vk::Pipeline m_pipeline;
public:
    virtual void UpdateUbo(const GameObject& p_gameObject) = 0;
    virtual void CreatePipeline(vk::Device p_logicalDevice) = 0;
    virtual void SetupVertexDescriptions() = 0;
    vk::Pipeline GetPipeline() {return m_pipeline;};
};


#endif //VULKANENGINE_ABSTRACTMATERIAL_HPP
