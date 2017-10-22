//
// Created by MTesseracT on 17-10-2017.
//

#ifndef VULKANENGINE_VULKANRENDERERBASE_HPP
#define VULKANENGINE_VULKANRENDERERBASE_HPP

//Vulkan
#include <vulkan/vulkan.hpp>

//GLFW
#include <GLFW/glfw3.h>

//GLM Related
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/hash.hpp>

//STB
#include <stb_image.h>

//STL/C libs
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <array>
#include <chrono>
#include <unordered_map>
#include <sstream>
#include <map>

//Own Classes
#include "Utility/Constants.hpp"
#include "WrappedVulkanDevice.hpp"
#include "Utility/Logger.hpp"
#include "WrappedVulkanValidation.hpp"
#include "VulkanHelpers.hpp"
#include "WrappedVulkanWindow.hpp"
#include "WrappedVulkanSwapchain.hpp"

class VulkanRendererBase
{
public:
    //Entry point
    void Initialize();
    void InitializeGlfwWindow();

    struct RendererSettings
    {
        bool validation = false;
        bool fullscreen = false;
        bool vsync = false;
    } m_settings;

protected:
private:
    WrappedVulkanWindow *               m_window                    = nullptr;
    int                                 m_windowHeight              = 720;
    int                                 m_windowWidth               = 1080;
    vk::Instance                        m_instance                  = nullptr;
    //Device related:
    vk::PhysicalDevice                  m_physicalDevice            = nullptr;
    vk::PhysicalDeviceProperties        m_deviceProperties;
    vk::PhysicalDeviceFeatures          m_deviceFeatures;
    vk::PhysicalDeviceMemoryProperties  m_deviceMemoryProperties;
    vk::PhysicalDeviceFeatures          m_enabledFeatures;
    vk::Device                          m_logicalDevice             = nullptr;
    std::vector<const char*>            m_enabledExtensions;
    WrappedVulkanDevice*                m_wrappedDevice             = nullptr;
    //Queue
    vk::Queue                           m_graphicsQueue             = nullptr;
    vk::Format                          m_depthFormat;
    vk::CommandPool                     m_commandPool;
    vk::PipelineStageFlags              m_submitPipelineStages;
    vk::SubmitInfo                      m_submitInfo;
    //Swapchain
    WrappedVulkanSwapchain              m_swapchain;

    void CreateInstance();
    bool CheckValidationLayerSupport();
    std::vector<const char *> GetRequiredExtensions();
    void CreateDebugCallback();
    void SelectPhysicalDevice();
    std::string GetDeviceTypeName(vk::PhysicalDeviceType p_type);
    void GetEnabledFeatures();
    void CreateLogicalDevice();
    void CreateSurface();
    void CreateSwapchain();
};


#endif //VULKANENGINE_VULKANRENDERERBASE_HPP
