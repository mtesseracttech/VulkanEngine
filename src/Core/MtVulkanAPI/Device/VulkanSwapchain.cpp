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

    m_queueFamilies = VulkanHelpers::FindQueueFamilies(m_physicalDevice, m_surface);
}
