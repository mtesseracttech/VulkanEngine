//
// Created by mtesseract on 8-2-18.
//

#include "VulkanPipelineLayout.hpp"
#include "VulkanUtility.hpp"

template <typename T>
void VulkanPipelineLayout::AddPushConstantRange(mtvk::ShaderStage p_stages)
{
    PushConstantRangeProperty rangeProperty{};
    rangeProperty.m_size = sizeof(T);

    if(p_stages & mtvk::ShaderStage::Vertex) rangeProperty.m_shaderStages | vk::ShaderStageFlagBits ::eVertex;
    if(p_stages & mtvk::ShaderStage::Fragment) rangeProperty.m_shaderStages | vk::ShaderStageFlagBits ::eFragment;

    m_pushConstantRanges.push_back(rangeProperty);
}

void VulkanPipelineLayout::Finalize()
{
    vk::PipelineLayoutCreateInfo layoutCreateInfo;
    layoutCreateInfo.pSetLayouts            = &m_descriptorSetLayout;
    layoutCreateInfo.setLayoutCount         = 1;
    layoutCreateInfo.pPushConstantRanges    = pushConstantRanges.data();
    layoutCreateInfo.pushConstantRangeCount = 2;
}
