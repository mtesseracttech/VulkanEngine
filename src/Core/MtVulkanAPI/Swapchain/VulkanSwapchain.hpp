//
// Created by MTesseracT on 16-2-2018.
//

#ifndef VULKANENGINE_VULKANSWAPCHAIN_HPP
#define VULKANENGINE_VULKANSWAPCHAIN_HPP


#include <vulkan/vulkan.hpp>

class VulkanSwapchain
{
private:
    struct
    {
        vk::SurfaceCapabilitiesKHR        capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR>   presentModes;
    } m_supportDetails;

    vk::PhysicalDevice m_physicalDevice;
    vk::SurfaceKHR     m_surface;

    QueueFamilyIndices m_queueFamilies;

    void QuerySwapchainSupport();

    vk::SurfaceFormatKHR ChooseSurfaceFormat();

    vk::PresentModeKHR ChoosePresentMode();

    vk::Extent2D ChooseExtent();

public:

    void Create(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface);

    const vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities();

    const std::vector<vk::SurfaceFormatKHR> &GetSurfaceFormats();

    const std::vector<vk::PresentModeKHR> &GetPresentModes();
};


#endif //VULKANENGINE_VULKANSWAPCHAIN_HPP
