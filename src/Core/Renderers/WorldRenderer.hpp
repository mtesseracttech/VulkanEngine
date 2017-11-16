//
// Created by MTesseracT on 16-11-2017.
//

#ifndef VULKANENGINE_WORLDRENDERER_HPP
#define VULKANENGINE_WORLDRENDERER_HPP


#include <Core/GameWorld.hpp>
#include <Core/MtVulkanAPI/VulkanRendererBase.hpp>

class WorldRenderer : public VulkanRendererBase
{
protected:

    GameWorld * m_gameWorld;

    const uint32_t VERTEX_BUFFER_BIND_ID = 0;

    vk::PipelineLayout m_pipelineLayout;

    //Set if the gameworld has changed and the buffers need to be refreshed
    bool m_needsBuild;
public:
    void BuildCommandBuffers();
};


#endif //VULKANENGINE_WORLDRENDERER_HPP
