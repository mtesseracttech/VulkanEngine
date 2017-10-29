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

    for (auto &format : depthFormats)
    {
        vk::FormatProperties formatProps = p_physicalDevice.getFormatProperties(format);

        if (formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
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
    for (const auto &queueFamily : queueFamilies)
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

        if (indices.IsComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

SwapChainSupportDetails VulkanHelpers::QuerySwapChainSupport(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface)
{
    SwapChainSupportDetails details{
            p_device.getSurfaceCapabilitiesKHR(p_surface),
            p_device.getSurfaceFormatsKHR(p_surface),
            p_device.getSurfacePresentModesKHR(p_surface)
    };
    return details;
}

std::string VulkanHelpers::GetDeviceTypeName(vk::PhysicalDeviceType p_type)
{
    switch (p_type)
    {
        case vk::PhysicalDeviceType::eOther:
            return "Other";
        case vk::PhysicalDeviceType::eIntegratedGpu:
            return "Integrated";
        case vk::PhysicalDeviceType::eDiscreteGpu:
            return "Discrete";
        case vk::PhysicalDeviceType::eVirtualGpu:
            return "Virtual";
        case vk::PhysicalDeviceType::eCpu:
            return "CPU";
    }
}

bool VulkanHelpers::CheckDeviceExtensionSupport(vk::PhysicalDevice p_device, std::vector<const char *> p_reqExtensions)
{
    std::vector<vk::ExtensionProperties> availableExtensions = p_device.enumerateDeviceExtensionProperties(nullptr);

    std::set<std::string> requiredExtensions(p_reqExtensions.begin(), p_reqExtensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


//Taken from https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanTools.cpp, only changed it to fit Vulkan.hpp style
void VulkanHelpers::SetImageLayout(
        vk::CommandBuffer p_commandbuffer,
        vk::Image p_image,
        vk::ImageLayout p_oldImageLayout,
        vk::ImageLayout p_newImageLayout,
        vk::ImageSubresourceRange p_subResourceRange,
        vk::PipelineStageFlags p_sourceStageMask,
        vk::PipelineStageFlags p_destinationStageMask)
{
    // Create an image barrier object
    vk::ImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.oldLayout = p_oldImageLayout;
    imageMemoryBarrier.newLayout = p_newImageLayout;
    imageMemoryBarrier.image = p_image;
    imageMemoryBarrier.subresourceRange = p_subResourceRange;

    // Source layouts (old)
    // Source access mask controls actions that have to be finished on the old layout
    // before it will be transitioned to the new layout
    switch (p_oldImageLayout)
    {
        case vk::ImageLayout::eUndefined:
            // Image layout is undefined (or does not matter)
            // Only valid as initial layout
            // No flags required, listed only for completeness
            imageMemoryBarrier.srcAccessMask = static_cast<vk::AccessFlagBits>(0);
            break;

        case vk::ImageLayout::ePreinitialized:
            // Image is preinitialized
            // Only valid as initial layout for linear images, preserves memory contents
            // Make sure host writes have been finished
            imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
            break;

        case vk::ImageLayout::eColorAttachmentOptimal:
            // Image is a color attachment
            // Make sure any writes to the color buffer have been finished
            imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            break;

        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            // Image is a depth/stencil attachment
            // Make sure any writes to the depth/stencil buffer have been finished
            imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            break;

        case vk::ImageLayout::eTransferSrcOptimal:
            // Image is a transfer source
            // Make sure any reads from the image have been finished
            imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
            break;

        case vk::ImageLayout::eTransferDstOptimal:
            // Image is a transfer destination
            // Make sure any writes to the image have been finished
            imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            break;

        case vk::ImageLayout::eShaderReadOnlyOptimal:
            // Image is read by a shader
            // Make sure any shader reads from the image have been finished
            imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
            break;
        default:
            // Other source layouts aren't handled (yet)
            break;
    }

    // Target layouts (new)
    // Destination access mask controls the dependency for the new image layout
    switch (p_newImageLayout)
    {
        case vk::ImageLayout::eTransferDstOptimal:
            // Image will be used as a transfer destination
            // Make sure any writes to the image have been finished
            imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
            break;

        case vk::ImageLayout::eTransferSrcOptimal:
            // Image will be used as a transfer source
            // Make sure any reads from the image have been finished
            imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
            break;

        case vk::ImageLayout::eColorAttachmentOptimal:
            // Image will be used as a color attachment
            // Make sure any writes to the color buffer have been finished
            imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            break;

        case vk::ImageLayout::eDepthStencilAttachmentOptimal:
            // Image layout will be used as a depth/stencil attachment
            // Make sure any writes to depth/stencil buffer have been finished
            imageMemoryBarrier.dstAccessMask =
                    imageMemoryBarrier.dstAccessMask | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            break;

        case vk::ImageLayout::eShaderReadOnlyOptimal:
            // Image will be read in a shader (sampler, input attachment)
            // Make sure any writes to the image have been finished
            if (imageMemoryBarrier.srcAccessMask == static_cast<vk::AccessFlagBits>(0))
            {
                imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eHostWrite | vk::AccessFlagBits::eTransferWrite;
            }
            imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
            break;
        default:
            // Other destination layouts aren't handled (yet)
            break;
    }

    // Put barrier inside setup command buffer
    p_commandbuffer.pipelineBarrier(p_sourceStageMask,
                                    p_destinationStageMask,
                                    static_cast<vk::DependencyFlagBits >(0),
                                    0, nullptr,
                                    0, nullptr,
                                    1, &imageMemoryBarrier);
}

void VulkanHelpers::SetImageLayout(vk::CommandBuffer p_commandbuffer,
                                   vk::Image p_image,
                                   vk::ImageAspectFlags p_aspectMask,
                                   vk::ImageLayout p_oldImageLayout,
                                   vk::ImageLayout p_newImageLayout,
                                   vk::PipelineStageFlags p_sourceStageMask,
                                   vk::PipelineStageFlags p_destinationStageMask)
{
    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = p_aspectMask;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;
    SetImageLayout(p_commandbuffer, p_image, p_oldImageLayout, p_newImageLayout, subresourceRange, p_sourceStageMask,
                   p_destinationStageMask);
}