//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANHELPERS_HPP
#define VULKANENGINE_VULKANHELPERS_HPP

#include <vulkan/vulkan.hpp>
#include <set>

struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool IsComplete()
    { return graphicsFamily >= 0 && presentFamily >= 0; }
};

class VulkanHelpers
{
public:
    static vk::Bool32 GetSupportedDepthFormat(vk::PhysicalDevice p_physicalDevice, vk::Format *p_depthFormat);

    static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface);

    static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface);

    static std::string GetDeviceTypeName(vk::PhysicalDeviceType p_type);

    static bool CheckDeviceExtensionSupport(vk::PhysicalDevice p_device, std::vector<const char *> p_reqExtensions);

    static void SetImageLayout(vk::CommandBuffer p_commandbuffer,
                               vk::Image p_image,
                               vk::ImageLayout p_oldImageLayout,
                               vk::ImageLayout p_newImageLayout,
                               vk::ImageSubresourceRange p_subResourceRange,
                               vk::PipelineStageFlags p_sourceStageMask = vk::PipelineStageFlagBits::eAllCommands,
                               vk::PipelineStageFlags p_destinationStageMask = vk::PipelineStageFlagBits::eAllCommands);

    static void SetImageLayout(vk::CommandBuffer p_commandbuffer,
                               vk::Image p_image,
                               vk::ImageAspectFlags p_aspectMask,
                               vk::ImageLayout p_oldImageLayout,
                               vk::ImageLayout p_newImageLayout,
                               vk::PipelineStageFlags p_sourceStageMask = vk::PipelineStageFlagBits::eAllCommands,
                               vk::PipelineStageFlags p_destinationStageMask = vk::PipelineStageFlagBits::eAllCommands);

    static vk::ShaderModule LoadShader(const char *p_fileName, vk::Device p_device);
};


#endif //VULKANENGINE_VULKANHELPERS_HPP
