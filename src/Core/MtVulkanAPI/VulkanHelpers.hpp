//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANHELPERS_HPP
#define VULKANENGINE_VULKANHELPERS_HPP

#include <vulkan/vulkan.hpp>

class VulkanHelpers
{
public:
		static vk::Bool32 GetSupportedDepthFormat(vk::PhysicalDevice p_physicalDevice, vk::Format *p_depthFormat);
};


#endif //VULKANENGINE_VULKANHELPERS_HPP
