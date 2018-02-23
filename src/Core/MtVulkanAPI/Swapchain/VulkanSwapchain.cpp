//
// Created by MTesseracT on 16-2-2018.
//

#include <Core/MtVulkanAPI/VulkanHelpers.hpp>
#include "VulkanSwapchain.hpp"

void VulkanSwapchain::Create(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface)
{
    assert(p_device && p_surface);

    m_physicalDevice = p_device;
    m_surface        = p_surface;
    m_queueFamilies  = VulkanHelpers::FindQueueFamilies(m_physicalDevice, m_surface);

    QuerySwapchainSupport();
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
    if(m_supportDetails.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
        return m_supportDetails.capabilities.currentExtent;
    }
    else{

    }
}

