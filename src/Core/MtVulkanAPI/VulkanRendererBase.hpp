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
    virtual //Entry point
    void Initialize();
    void InitializeGlfwWindow();
    GLFWwindow * GetWindow();

    struct RendererSettings
    {
        bool validation = false;
        bool fullscreen = false;
        bool vsync = false;
    } m_settings;

    void DrawFrame();

    virtual void Render() = 0;

    void Cleanup();

    VulkanRendererBase();

    void DeviceWaitIdle();

    virtual ~VulkanRendererBase();

protected:

    vk::Instance                        m_instance                  = nullptr;
    //Device related:
    vk::PhysicalDevice                  m_physicalDevice            = nullptr;
    vk::PhysicalDeviceProperties        m_deviceProperties;
    vk::PhysicalDeviceFeatures          m_deviceFeatures;
    vk::PhysicalDeviceMemoryProperties  m_deviceMemoryProperties;
    vk::PhysicalDeviceFeatures          m_enabledFeatures;
    vk::Device                          m_logicalDevice             = nullptr;
    //Extensions
    std::vector<const char*>            m_deviceExtensions          = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    //Queue
    vk::Queue                           m_graphicsQueue             = nullptr;
    vk::Format                          m_depthFormat;
    vk::RenderPass                      m_renderPass;
    //Command Buffers
    std::vector<vk::CommandBuffer>      m_drawCmdBuffers;

    vk::PipelineCache                   m_pipelineCache;
    std::vector<vk::Framebuffer>        m_frameBuffers;
    uint32_t                            m_currentBuffer;
    std::vector<vk::ShaderModule>       m_shaderModules;
    vk::DescriptorPool                  m_descriptorPool            = nullptr;
    vk::ClearColorValue                 m_defaultClearColor         = {{0.1f, 0.1f,0.1f,1.0f}};

    struct {
        // Swap chain image presentation
        vk::Semaphore presentComplete;
        // Command buffer submission and execution
        vk::Semaphore renderComplete;
    } m_semaphores;

    struct
    {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
    } m_depthStencil;

private:
    int                                 m_windowHeight              = 720;
    int                                 m_windowWidth               = 1080;
    WrappedVulkanWindow *               m_window                    = nullptr;
    WrappedVulkanDevice *               m_wrappedDevice             = nullptr;

    //Swapchain
    WrappedVulkanSwapchain              m_swapchain;
    vk::CommandPool                     m_commandPool;


protected:
    void CreateInstance();

    bool CheckValidationLayerSupport();

    std::vector<const char *> GetRequiredExtensions();

    void CreateDebugCallback();

    void SelectPhysicalDevice();

    virtual void GetEnabledFeatures();

    void CreateLogicalDevice();

    void CreateSurface();

    void ConnectSwapchain();

    void CreateImageViews();

    void SetupRenderPass();

    bool IsDeviceSuitable(vk::PhysicalDevice p_device);

    void CreateCommandPool();

    void CreateSwapchain();

    void CreateCommandBuffers();

    void Prepare();

    void SetupDepthStencil();

    void CreatePipelineCache();

    void SetupFrameBuffer();

    void CleanupSwapchain();

    vk::PipelineShaderStageCreateInfo LoadShader(const std::string &p_fileName, vk::ShaderStageFlagBits p_shaderStage);

    void PrepareFrame();

    void SubmitFrame();
};


#endif //VULKANENGINE_VULKANRENDERERBASE_HPP
