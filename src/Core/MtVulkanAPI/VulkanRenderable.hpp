//
// Created by MTesseracT on 15-11-2017.
//

#ifndef VULKANENGINE_VULKANRENDERABLE_HPP
#define VULKANENGINE_VULKANRENDERABLE_HPP


#include <vulkan/vulkan.hpp>
#include <Core/Renderers/WorldRenderer.hpp>
#include "VulkanModel.hpp"
#include "VulkanShaderProgram.hpp"

class VulkanRenderable
{
protected:
    vk::DescriptorSet   m_descriptorSet;
    vk::Pipeline        m_pipeline;
    VulkanModel         m_model;
    WrappedVulkanBuffer m_uniformBuffer;

    VulkanDevice * m_wrappedDevice;
public:
    void BindDrawData(vk::CommandBuffer p_drawCommandBuffer, vk::PipelineLayout p_pipelineLayout, uint32_t p_vertexBufferBinding, vk::DeviceSize* p_offset);
    void CreateUbo(VulkanDevice *p_device, uint32_t uboSize);
};


#endif //VULKANENGINE_VULKANRENDERABLE_HPP
