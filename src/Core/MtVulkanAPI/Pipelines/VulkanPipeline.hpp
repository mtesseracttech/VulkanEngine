//
// Created by mtesseract on 8-2-18.
//

#ifndef VULKANENGINE_VULKANPIPELINE_HPP
#define VULKANENGINE_VULKANPIPELINE_HPP


#include <vulkan/vulkan.hpp>

class VulkanPipeline
{
protected:
    vk::Pipeline m_pipeline;
public:
    const vk::Pipeline Get();
};


#endif //VULKANENGINE_VULKANPIPELINE_HPP
