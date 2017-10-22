//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_WRAPPEDVULKANSWAPCHAIN_HPP
#define VULKANENGINE_WRAPPEDVULKANSWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>
#include "WrappedVulkanDevice.hpp"
#include "WrappedVulkanWindow.hpp"

class WrappedVulkanSwapchain
{
    vk::Device                  m_device;
    vk::PhysicalDevice          m_physicalDevice;
    vk::SwapchainKHR            m_swapchain = nullptr;
    vk::Format                  m_imageFormat;
    vk::Extent2D                m_swapchainExtent;
    std::vector<vk::Image>      m_images;
    std::vector<vk::ImageView>  m_swapChainImageViews;

    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR          capabilities;
        std::vector<vk::SurfaceFormatKHR>   formats;
        std::vector<vk::PresentModeKHR>     presentModes;
    };

public:
    void Create(WrappedVulkanDevice * p_device, WrappedVulkanWindow * p_window)
    {
        Logger::Log("Creating swapchain");
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(p_device->m_physicalDevice, p_window->m_surface);

        vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, *p_window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.surface = p_window->m_surface;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

        QueueFamilyIndices indices = VulkanHelpers::FindQueueFamilies(p_device->m_physicalDevice, p_window->m_surface);

        uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

        if (p_device->m_queueFamilyIndices.graphics != p_device->m_queueFamilyIndices.transfer)
        {
            swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;

        vk::CompositeAlphaFlagBitsKHR compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

        std::vector<vk::CompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
                vk::CompositeAlphaFlagBitsKHR::eOpaque,
                vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
                vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
                vk::CompositeAlphaFlagBitsKHR::eInherit
        };

        for (auto& compositeAlphaFlag : compositeAlphaFlags)
        {
            if (swapChainSupport.capabilities.supportedCompositeAlpha & compositeAlphaFlag)
            {
                compositeAlpha = compositeAlphaFlag;
                break;
            };
        }

        swapchainCreateInfo.compositeAlpha = compositeAlpha;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;

        vk::FormatProperties formatProperties = p_device->m_physicalDevice.getFormatProperties(surfaceFormat.format);

        if ((formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrcKHR) ||
            (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc))
        {
            swapchainCreateInfo.imageUsage |= vk::ImageUsageFlagBits::eTransferSrc;
        }

        m_swapchain = p_device->m_logicalDevice.createSwapchainKHR(swapchainCreateInfo);

        m_images = p_device->m_logicalDevice.getSwapchainImagesKHR(m_swapchain);
        m_imageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;
    }

private:
    SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice p_physicalDevice, vk::SurfaceKHR p_surface)
    {
        SwapChainSupportDetails details{
                p_physicalDevice.getSurfaceCapabilitiesKHR(p_surface),
                p_physicalDevice.getSurfaceFormatsKHR(p_surface),
                p_physicalDevice.getSurfacePresentModesKHR(p_surface)
        };
        return details;
    }



    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
    {
        if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
        {
            return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
        }

        for (const auto &availableFormat : availableFormats)
        {

            if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
                availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes)
    {
        vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

        for (const auto &availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox)
            {
                return availablePresentMode;
            }
            else if (availablePresentMode == vk::PresentModeKHR::eImmediate)
            {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }

    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities, const WrappedVulkanWindow& p_window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetWindowSize(p_window.m_window, &width, &height);

            vk::Extent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width,
                                          std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                                           std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }
};


#endif //VULKANENGINE_WRAPPEDVULKANSWAPCHAIN_HPP
