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
    WrappedVulkanWindow *       m_window;
    std::vector<vk::Image>      m_images;
    std::vector<vk::ImageView>  m_imageViews;

    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR          capabilities;
        std::vector<vk::SurfaceFormatKHR>   formats;
        std::vector<vk::PresentModeKHR>     presentModes;
    };

public:
    void Connect(vk::PhysicalDevice p_physicalDevice, vk::Device p_logicalDevice, WrappedVulkanWindow *p_window)
    {
        m_physicalDevice = p_physicalDevice;
        m_device = p_logicalDevice;
        m_window = p_window;
        Logger::Log("Swapchain connected!");
    }

    void Create()
    {
        Logger::Log("Creating swapchain");
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport();

        vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.surface = m_window->m_surface;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

        QueueFamilyIndices indices = VulkanHelpers::FindQueueFamilies(m_physicalDevice, m_window->m_surface);

        uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

        if (indices.graphicsFamily != indices.presentFamily)
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

        vk::FormatProperties formatProperties = m_physicalDevice.getFormatProperties(surfaceFormat.format);

        if ((formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eTransferSrcKHR) ||
            (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc))
        {
            swapchainCreateInfo.imageUsage |= vk::ImageUsageFlagBits::eTransferSrc;
        }

        m_swapchain = m_device.createSwapchainKHR(swapchainCreateInfo);

        m_images = m_device.getSwapchainImagesKHR(m_swapchain);
        m_imageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;
    }

    vk::ImageView &CreateImageView(vk::Image image, vk::ImageAspectFlagBits p_aspectFlags)
    {
        vk::ImageViewCreateInfo viewCreateInfo;
        viewCreateInfo.image = image;
        viewCreateInfo.viewType = vk::ImageViewType::e2D;
        viewCreateInfo.format = m_imageFormat;
        viewCreateInfo.subresourceRange.aspectMask = p_aspectFlags;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.subresourceRange.baseArrayLayer = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;

        vk::ImageView imageView = m_device.createImageView(viewCreateInfo);

        return imageView;
    }

    void CreateImageViews()
    {
        m_imageViews.resize(m_images.size());

        for (uint32_t i = 0; i < m_images.size(); ++i)
        {
            m_imageViews[i] = CreateImageView(m_images[i], vk::ImageAspectFlagBits::eColor);
        }
    }

private:
    SwapChainSupportDetails QuerySwapChainSupport()
    {
        SwapChainSupportDetails details{
                m_physicalDevice.getSurfaceCapabilitiesKHR(m_window->m_surface),
                m_physicalDevice.getSurfaceFormatsKHR(m_window->m_surface),
                m_physicalDevice.getSurfacePresentModesKHR(m_window->m_surface)
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

    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetWindowSize(m_window->m_window, &width, &height);

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
