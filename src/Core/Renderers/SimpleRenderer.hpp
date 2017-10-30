//
// Created by MTesseracT on 30-10-2017.
//

#ifndef VULKANENGINE_SIMPLERENDERER_HPP
#define VULKANENGINE_SIMPLERENDERER_HPP


#include <Core/MtVulkanAPI/VulkanRendererBase.hpp>
#include <Core/MtVulkanAPI/VulkanModel.hpp>
#include <Core/Camera.hpp>

class SimpleRenderer : public VulkanRendererBase
{
    VertexLayout m_vertexLayout = VertexLayout(
            {VulkanVertexComponent ::ePosition,
             VulkanVertexComponent ::eNormal,
             VulkanVertexComponent ::eUV,
             VulkanVertexComponent ::eColor}
    );

    struct {
        VulkanModel bunny;
        VulkanModel monkey;
        VulkanModel teapot;
    } m_models;

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    } m_ubo;

    vk::Pipeline                m_phong;
    vk::PipelineLayout          m_pipelineLayout;
    vk::DescriptorSet           m_descriptorSet;
    vk::DescriptorSetLayout     m_descriptorSetLayout;

    WrappedVulkanBuffer         m_uniformBuffer;

    Camera*                     m_camera;

public:
    SimpleRenderer();
    virtual ~SimpleRenderer();
    void GetEnabledFeatures() override;
    void Render() override;
};


#endif //VULKANENGINE_SIMPLERENDERER_HPP
