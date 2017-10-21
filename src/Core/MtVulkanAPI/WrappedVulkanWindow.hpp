//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
#define VULKANENGINE_VULKANWRAPPEDWINDOW_HPP

#include "Utility/Logger.hpp"
#include <GLFW/glfw3.h>
#include <exception>
#include <vulkan/vulkan.hpp>
#include "WrappedVulkanDevice.hpp"
#include <vector>

struct WrappedVulkanWindow
{
    GLFWwindow *m_window;
    vk::SurfaceKHR m_surface;

    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    WrappedVulkanWindow(int windowWidth, int windowHeight, std::string windowTitle, bool fullscreen)
    {
        Logger::Log("GLFW is initializing");
        if (glfwInit() == GLFW_TRUE)
        {
            Logger::Log("GLFW Initialized Successfully!");
        }
        else
        {
            throw std::runtime_error("GLFW Failed to initialize, terminating...");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        Logger::Log("GLFW set to load without an API, so Vulkan can be used");
        m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(),
                                    (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
        glfwSetWindowUserPointer(m_window, this);
    }

    void CreateSwapchain(vk::PhysicalDevice p_physicalDevice, WrappedVulkanDevice::QueueFamilyIndices p_queueFamilyIndices)
    {
        SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(p_physicalDevice);

        vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR swapchainCreateInfo;
        swapchainCreateInfo.surface = m_surface;
        swapchainCreateInfo.minImageCount = imageCount;
        swapchainCreateInfo.imageFormat = surfaceFormat.format;
        swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
        swapchainCreateInfo.imageExtent = extent;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;


    }

private:
    SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice p_physicalDevice)
    {
        SwapChainSupportDetails details{
                p_physicalDevice.getSurfaceCapabilitiesKHR(m_surface),
                p_physicalDevice.getSurfaceFormatsKHR(m_surface),
                p_physicalDevice.getSurfacePresentModesKHR(m_surface)
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
            glfwGetWindowSize(m_window, &width, &height);

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


#endif //VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
