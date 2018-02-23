//
// Created by MTesseracT on 12-2-2018.
//

#ifndef VULKANENGINE_DISPLAY_HPP
#define VULKANENGINE_DISPLAY_HPP


#include <Core/MtVulkanAPI/VulkanWindow.hpp>
#include <Core/MtVulkanAPI/VulkanRendererBase.hpp>
#include <Core/MtVulkanAPI/Debug/VulkanDebug.hpp>
#include "VulkanDevice.hpp"
#include "Core/MtVulkanAPI/Swapchain/VulkanSwapchain.hpp"
#include <memory>

class Display
{
private:
    Display();

    ~Display();

    vk::Instance    m_instance;
    VulkanWindow    m_window;
    VulkanDebug     m_debug;
    VulkanDevice    m_device;
    VulkanSwapchain m_swapchain;

    static void OnWindowResized(GLFWwindow *p_window, int p_width, int p_height);

    std::vector<const char *> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

protected:
    void CreateWindow();

    void CreateInstance();

public:
    static Display *Get();

    void Initialize();

    void Cleanup();

    void CreateDebugCallback();

    void SelectPhysicalDevice();

    void CreateLogicalDevice();

    int GetDeviceScore(vk::PhysicalDevice p_device);

    bool IsDeviceSuitable(vk::PhysicalDevice p_device);

    void CreateSurface();

    vk::Device GetDevice();

    VulkanDevice GetVulkanDevice();
};


#endif //VULKANENGINE_DISPLAY_HPP
