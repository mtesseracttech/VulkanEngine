//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANHELPERS_HPP
#define VULKANENGINE_VULKANHELPERS_HPP

#include <vulkan/vulkan.hpp>

struct SwapChainSupportDetails
{
	vk::SurfaceCapabilitiesKHR          capabilities;
	std::vector<vk::SurfaceFormatKHR>   formats;
	std::vector<vk::PresentModeKHR>     presentModes;
};

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() { return graphicsFamily >= 0 && presentFamily >= 0; }
};

class VulkanHelpers
{
public:
	static vk::Bool32 GetSupportedDepthFormat(vk::PhysicalDevice p_physicalDevice, vk::Format *p_depthFormat);
    static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface);
	static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface);
};


#endif //VULKANENGINE_VULKANHELPERS_HPP
