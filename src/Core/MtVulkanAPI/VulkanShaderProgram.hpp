//
// Created by MTesseracT on 8-11-2017.
//

#ifndef VULKANENGINE_VULKANSHADERPROGRAM_HPP
#define VULKANENGINE_VULKANSHADERPROGRAM_HPP

#include <vulkan/vulkan.hpp>

class VulkanShaderProgram
{
private:
    std::vector<vk::PipelineShaderStageCreateInfo> m_shaderStages;
protected:
public:
};

#endif //VULKANENGINE_VULKANSHADERPROGRAM_HPP
