//
// Created by mtesseract on 8-2-18.
//

#include "IRenderable.hpp"

#define VERTEX_BUFFER_BIND 0

void IRenderable::BuildCommandBuffer(vk::CommandBuffer p_cmdBuffer)
{
    std::array<vk::DeviceSize, 1> offsets = {0};

    m_material->BindPipeline(p_cmdBuffer);
    m_material->BindDescriptorSets(p_cmdBuffer);
    p_cmdBuffer.bindVertexBuffers(VERTEX_BUFFER_BIND, 1, &m_model->m_vertexBuffer.m_buffer, offsets.data());
    p_cmdBuffer.bindIndexBuffer(m_model->m_indexBuffer.m_buffer, vk::DeviceSize(0), vk::IndexType::eUint32);
    m_material->UpdatePushConstants(p_cmdBuffer);

    p_cmdBuffer.drawIndexed(m_model->m_indexCount, 1, 0,0,0);
}

void IRenderable::SetMaterial(AbstractMaterial *p_material)
{
    m_material = p_material;
}

void IRenderable::SetModel(VulkanModel *p_model)
{
    m_model = p_model;
}

void IRenderable::UpdateUniforms()
{
    m_material->UpdateUbo();
}
