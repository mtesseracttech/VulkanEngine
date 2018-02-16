//
// Created by MTesseracT on 16-2-2018.
//

#include "VulkanSwapchain.hpp"

QueueFamilies VulkanSwapchain::FindQueueFamilies()
{

}

void VulkanSwapchain::Initialize(vk::PhysicalDevice p_device, vk::SurfaceKHR p_surface)
{
    assert(p_device && p_surface);

    m_physicalDevice = p_device;
    m_surface        = p_surface;
}
