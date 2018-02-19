//
// Created by MTesseracT on 16-2-2018.
//

#ifndef VULKANENGINE_VULKANSWAPCHAIN_HPP
#define VULKANENGINE_VULKANSWAPCHAIN_HPP


#include <vulkan/vulkan.hpp>

class VulkanSwapchain
{
private:
    vk::PhysicalDevice m_physicalDevice;
    vk::SurfaceKHR     m_surface;

    QueueFamilyIndices m_queueFamilies;

public:

    void Create(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface);
};


#endif //VULKANENGINE_VULKANSWAPCHAIN_HPP
