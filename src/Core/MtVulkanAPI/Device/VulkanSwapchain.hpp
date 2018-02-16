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
    vk::SurfaceKHR m_surface;


public:
    struct QueueFamilyIndices {
        int graphicsFamily = -1;
        int presentFamily = -1;

        bool IsComplete() { return graphicsFamily >= 0 && presentFamily >= 0; }
    } m_queueFamilies;
};


#endif //VULKANENGINE_VULKANSWAPCHAIN_HPP
