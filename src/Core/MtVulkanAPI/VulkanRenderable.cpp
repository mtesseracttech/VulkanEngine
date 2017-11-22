//
// Created by MTesseracT on 15-11-2017.
//

#include "VulkanRenderable.hpp"

//Creates command buffer data for object command buffer creation
void VulkanRenderable::BindDrawData(vk::CommandBuffer p_drawCommandBuffer, vk::PipelineLayout p_pipelineLayout, uint32_t p_vertexBufferBinding, vk::DeviceSize * p_offset)
{
    p_drawCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                           p_pipelineLayout,
                                           0,
                                           1,
                                           &m_descriptorSet,
                                           0,
                                           nullptr);

    p_drawCommandBuffer.bindVertexBuffers(p_vertexBufferBinding,
                                          1,
                                          &m_model.m_vertexBuffer.m_buffer,
                                          p_offset);

    p_drawCommandBuffer.bindIndexBuffer(m_model.m_indexBuffer.m_buffer,
                                        0,
                                        vk::IndexType::eUint32);

    p_drawCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline);

    p_drawCommandBuffer.drawIndexed(m_model.m_indexCount, 1, 0, 0, 0);
}

//Creates an empty ubo and maps it
void VulkanRenderable::CreateUbo(WrappedVulkanDevice * p_device, uint32_t uboSize)
{
    p_device->CreateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                           vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                           &m_uniformBuffer,
                           uboSize);

    m_uniformBuffer.Map();
}