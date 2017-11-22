//
// Created by MTesseracT on 16-11-2017.
//

#include "WorldRenderer.hpp"
#include <Core/GameWorld.hpp>

void WorldRenderer::BuildCommandBuffers()
{
    Logger::Log("Building Command Buffers");
    vk::CommandBufferBeginInfo commandBufferBeginInfo;
    //commandBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits ::eSimultaneousUse;

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color = vk::ClearColorValue(std::array<float,4>{0.1f, 0.1f,0.1f,1.0f});
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);

    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass        = m_renderPass;
    renderPassBeginInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderPassBeginInfo.renderArea.extent = m_swapchain.GetExtent();
    renderPassBeginInfo.clearValueCount   = clearValues.size();
    renderPassBeginInfo.pClearValues      = clearValues.data();

    for (int32_t i = 0; i < m_drawCommandBuffers.size(); ++i)
    {
        renderPassBeginInfo.framebuffer = m_frameBuffers[i];
        m_drawCommandBuffers[i].begin(commandBufferBeginInfo);

        m_drawCommandBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport;
        viewport.width = m_swapchain.GetExtent().width;
        viewport.height = m_swapchain.GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        m_drawCommandBuffers[i].setViewport(0, 1, &viewport);

        vk::Rect2D scissor;
        scissor.extent = m_swapchain.GetExtent();
        scissor.offset = vk::Offset2D(0, 0);

        m_drawCommandBuffers[i].setScissor(0, 1, &scissor);

        vk::DeviceSize offsets[1] = {0};

        //Skybox

        /*
        m_drawCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1,
                                                   &m_descriptorSets.skybox, 0, nullptr);

        m_drawCommandBuffers[i].bindVertexBuffers(VERTEX_BUFFER_BIND_ID, 1, &m_models.skybox.m_vertexBuffer.m_buffer, offsets);

        m_drawCommandBuffers[i].bindIndexBuffer(m_models.skybox.m_indexBuffer.m_buffer, 0, vk::IndexType::eUint32);

        m_drawCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelines.skybox);

        m_drawCommandBuffers[i].drawIndexed(m_models.skybox.m_indexCount, 1, 0, 0, 0);
        */

        //Render Game World

        m_gameWorld->GenerateCommandBufferInfo(m_drawCommandBuffers[i], m_pipelineLayout, VERTEX_BUFFER_BIND_ID, offsets);

        //End render pass

        m_drawCommandBuffers[i].endRenderPass();

        m_drawCommandBuffers[i].end();
    }
}