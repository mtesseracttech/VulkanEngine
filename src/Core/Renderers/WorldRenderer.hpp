//
// Created by MTesseracT on 16-11-2017.
//

#ifndef VULKANENGINE_WORLDRENDERER_HPP
#define VULKANENGINE_WORLDRENDERER_HPP


#include <Core/MtVulkanAPI/VulkanRendererBase.hpp>

class GameWorld;
//#include <Core/GameWorld.hpp>

class WorldRenderer : public VulkanRendererBase
{
    const uint32_t VERTEX_BUFFER_BIND_ID = 0;
    vk::PipelineLayout m_pipelineLayout;
    //Set if the gameworld has changed and the buffers need to be refreshed
    bool m_needsBuild = true;
public:
    void BuildCommandBuffers();

    GameWorld * m_gameWorld;
};


#endif //VULKANENGINE_WORLDRENDERER_HPP
