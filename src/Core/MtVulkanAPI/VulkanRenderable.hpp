//
// Created by MTesseracT on 15-11-2017.
//

#ifndef VULKANENGINE_VULKANRENDERABLE_HPP
#define VULKANENGINE_VULKANRENDERABLE_HPP


#include <vulkan/vulkan.hpp>
#include "VulkanModel.hpp"
#include "VulkanShaderProgram.hpp"

class VulkanRenderable
{
protected:
    vk::DescriptorSet   m_descriptorSet;
    vk::Pipeline        m_pipeline;
    VulkanModel         m_model;
    WrappedVulkanBuffer m_uniformBuffer;
public:
    void BindDrawData(vk::CommandBuffer p_drawCommandBuffer, vk::PipelineLayout p_pipelineLayout, uint32_t p_vertexBufferBinding, vk::DeviceSize* p_offset);
    void CreateUbo(WrappedVulkanDevice *p_device, uint32_t uboSize);
    void LoadModel();
};


#endif //VULKANENGINE_VULKANRENDERABLE_HPP
