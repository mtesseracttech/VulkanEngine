//
// Created by MTesseracT on 2-11-2017.
//

#ifndef VULKANENGINE_ABSTRACTMATERIAL_HPP
#define VULKANENGINE_ABSTRACTMATERIAL_HPP


#include <vulkan/vulkan.hpp>
#include <Core/MtVulkanAPI/VulkanTexture.hpp>
#include <Core/GameObject.hpp>
#include <Core/MtVulkanAPI/Pipelines/VulkanPipeline.hpp>
#include <Core/MtVulkanAPI/VulkanPipelineLayout.hpp>
#include <Core/MtVulkanAPI/VulkanDescriptorSet.hpp>

class AbstractMaterial
{
protected:
    VulkanPipelineLayout *m_pipelineLayout;
    VulkanPipeline       *m_pipeline;
    VulkanDescriptorSet  *m_descriptorSet;

public:
    virtual void BindPipeline(vk::CommandBuffer p_cmdBuffer)
    {
        p_cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline->Get());
    };

    //static virtual void SetupPipelineLayout() = 0;
    //static virtual void SetupPipeline() = 0;
    virtual void BindDescriptorSets(vk::CommandBuffer p_cmdBuffer) = 0;

    virtual void UpdatePushConstants(vk::CommandBuffer p_cmdBuffer) = 0;

    virtual void UpdateUbo() = 0;
};


#endif //VULKANENGINE_ABSTRACTMATERIAL_HPP
