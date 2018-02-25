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

    vk::Device                 m_logicalDevice;
    vk::PhysicalDevice         m_physicalDevice;
    vk::SurfaceKHR             m_surface;
    vk::SwapchainKHR           m_swapchain;
    std::vector<vk::Image>     m_images;
    std::vector<vk::ImageView> m_imageViews;
    vk::Format                 m_imageFormat;
    vk::Extent2D               m_extent;
    QueueFamilyIndices         m_queueFamilies;

    void QuerySwapchainSupport();

    vk::SurfaceFormatKHR ChooseSurfaceFormat();

    vk::PresentModeKHR ChoosePresentMode();

    vk::Extent2D ChooseExtent();

    void CreateImageViews();

    vk::ImageView CreateImageView(vk::Image p_image, vk::ImageAspectFlags p_aspectFlags);
public:

    void Initialize(VulkanDevice &p_device, vk::SurfaceKHR p_surface);

    void Create();

    const vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities();

    const std::vector<vk::SurfaceFormatKHR> &GetSurfaceFormats();

    const std::vector<vk::PresentModeKHR> &GetPresentModes();

};


#endif //VULKANENGINE_VULKANSWAPCHAIN_HPP
