//
// Created by MTesseracT on 21-10-2017.
//

#include "VulkanHelpers.hpp"

vk::Bool32 VulkanHelpers::GetSupportedDepthFormat(vk::PhysicalDevice p_physicalDevice, vk::Format *p_depthFormat)
{
    std::vector<vk::Format> depthFormats = {
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD32Sfloat,
            vk::Format::eD24UnormS8Uint,
            vk::Format::eD16UnormS8Uint,
            vk::Format::eD16Unorm
    };

    for (auto& format : depthFormats)
    {
        vk::FormatProperties formatProps = p_physicalDevice.getFormatProperties(format);

        if(formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
        {
            *p_depthFormat = format;
            return static_cast<vk::Bool32>(true);
        }
    }

    return static_cast<vk::Bool32>(false);
}

QueueFamilyIndices VulkanHelpers::FindQueueFamilies(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface)
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = p_device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphicsFamily = i;
        }

        vk::Bool32 presentSupport = p_device.getSurfaceSupportKHR(i, p_surface);

        if (queueFamily.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}