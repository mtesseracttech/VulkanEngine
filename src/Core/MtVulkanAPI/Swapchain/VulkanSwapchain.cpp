//
// Created by MTesseracT on 16-2-2018.
//

#include <Core/MtVulkanAPI/VulkanHelpers.hpp>
#include <Core/MtVulkanAPI/Window/VulkanWindow.hpp>
#include "VulkanSwapchain.hpp"

void VulkanSwapchain::Initialize(VulkanDevice &p_device, vk::SurfaceKHR p_surface)
{
    assert(p_device.m_physicalDevice && p_device.m_logicalDevice && p_surface);

    m_physicalDevice = p_device.m_physicalDevice;
    m_logicalDevice  = p_device.m_logicalDevice;
    m_surface        = p_surface;
    m_queueFamilies  = VulkanHelpers::FindQueueFamilies(m_physicalDevice, m_surface);
}

void VulkanSwapchain::Create()
{
    QuerySwapchainSupport();

    vk::SurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat();
    vk::PresentModeKHR   presentMode   = ChoosePresentMode();
    vk::Extent2D         extent        = ChooseExtent();

    uint32_t imageCount = m_supportDetails.capabilities.minImageCount + 1;

    if (m_supportDetails.capabilities.maxImageCount > 0 && imageCount > m_supportDetails.capabilities.maxImageCount)
    {
        imageCount = m_supportDetails.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface          = m_surface;
    createInfo.minImageCount    = imageCount;
    createInfo.imageFormat      = surfaceFormat.format;
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;
    createInfo.imageExtent      = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage       = vk::ImageUsageFlagBits::eColorAttachment;
    createInfo.presentMode      = presentMode;
    createInfo.preTransform     = m_supportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha   = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.clipped          = vk::Bool32(true);

    std::array<uint32_t, 2> indices = {static_cast<uint32_t>(m_queueFamilies.graphicsFamily),
                                       static_cast<uint32_t>(m_queueFamilies.presentFamily)};

    if (m_queueFamilies.graphicsFamily != m_queueFamilies.presentFamily)
    {
        createInfo.imageSharingMode      = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = indices.size();
        createInfo.pQueueFamilyIndices   = indices.data();
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    m_swapchain = m_logicalDevice.createSwapchainKHR(createInfo);
    m_images = m_logicalDevice.getSwapchainImagesKHR(m_swapchain);

    m_imageFormat = surfaceFormat.format;
    m_extent = extent;

    CreateImageViews();
}

void VulkanSwapchain::QuerySwapchainSupport()
{
    assert(m_physicalDevice && m_surface);

    m_supportDetails.capabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);
    m_supportDetails.formats      = m_physicalDevice.getSurfaceFormatsKHR(m_surface);
    m_supportDetails.presentModes = m_physicalDevice.getSurfacePresentModesKHR(m_surface);
}

const vk::SurfaceCapabilitiesKHR VulkanSwapchain::GetSurfaceCapabilities()
{
    return m_supportDetails.capabilities;
}

const std::vector<vk::SurfaceFormatKHR> &VulkanSwapchain::GetSurfaceFormats()
{
    return m_supportDetails.formats;
}

const std::vector<vk::PresentModeKHR> &VulkanSwapchain::GetPresentModes()
{
    return m_supportDetails.presentModes;
}

vk::SurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat()
{
    assert(!m_supportDetails.formats.empty());

    if (m_supportDetails.formats.size() == 1 && m_supportDetails.formats[0].format == vk::Format::eUndefined)
    {
        return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
    }

    for (const auto &availableFormat : m_supportDetails.formats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return availableFormat;
        }
    }

    return m_supportDetails.formats[0];
}

vk::PresentModeKHR VulkanSwapchain::ChoosePresentMode()
{
    assert(!m_supportDetails.presentModes.empty());

    vk::PresentModeKHR optimalPresentMode = vk::PresentModeKHR::eFifo;

    for (const auto &availablePresentMode : m_supportDetails.presentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
        else if (availablePresentMode == vk::PresentModeKHR::eImmediate)
        {
            optimalPresentMode = availablePresentMode;
        }
    }

    return optimalPresentMode;
}

vk::Extent2D VulkanSwapchain::ChooseExtent()
{
    if (m_supportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return m_supportDetails.capabilities.currentExtent;
    }
    else
    {
        //Get size from the window
        VulkanWindow *window    = VulkanWindow::Get();
        glm::ivec2   windowSize = window->GetWindowSize();

        vk::Extent2D realExtent(static_cast<uint32_t >(windowSize.x), static_cast<uint32_t >(windowSize.y));

        vk::Extent2D minImageExtent = m_supportDetails.capabilities.minImageExtent;
        vk::Extent2D maxImageExtent = m_supportDetails.capabilities.maxImageExtent;

        realExtent.width  = std::max(minImageExtent.width, std::min(maxImageExtent.width, realExtent.width));
        realExtent.height = std::max(minImageExtent.height, std::min(maxImageExtent.height, realExtent.height));

        return realExtent;
    }
}

void VulkanSwapchain::CreateImageViews()
{
    m_imageViews.resize(m_images.size());

    for (uint32_t i= 0; i < m_images.size(); ++i)
    {
        m_imageViews[i] = CreateImageView(m_images[i], vk::ImageAspectFlagBits::eColor);
    }
}

vk::ImageView VulkanSwapchain::CreateImageView(vk::Image p_image, vk::ImageAspectFlags p_aspectFlags)
{
    vk::ImageViewCreateInfo createInfo;
    createInfo.image = p_image;
    createInfo.format = m_imageFormat;
    createInfo.viewType = vk::ImageViewType ::e2D;
    createInfo.subresourceRange.aspectMask = p_aspectFlags;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    return m_logicalDevice.createImageView(createInfo);
}

