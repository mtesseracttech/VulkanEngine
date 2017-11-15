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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//STB
//#include <stb_image.h>

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
    struct RendererSettings
    {
        bool validation = false;
        bool fullscreen = false;
        bool vsync = false;
    } m_settings;
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
    std::vector<vk::CommandBuffer>      m_drawCommandBuffers;

    vk::PipelineCache                   m_pipelineCache;
    std::vector<vk::Framebuffer>        m_frameBuffers;
    uint32_t                            m_currentBuffer;
    std::vector<vk::ShaderModule>       m_shaderModules;
    vk::DescriptorPool                  m_descriptorPool            = nullptr;
    vk::ClearColorValue                 m_defaultClearColor         = std::array<float, 4>{0.1f, 0.1f,0.1f,1.0f};

    //Swapchain
    WrappedVulkanSwapchain              m_swapchain;
    vk::CommandPool                     m_commandPool;

    WrappedVulkanWindow *               m_window                    = nullptr;
    WrappedVulkanDevice *               m_wrappedDevice             = nullptr;

    //Contains command buffers and semaphores to be presented to the queue
    vk::SubmitInfo                      m_submitInfo;
    vk::PipelineStageFlags              m_submitPipelineStages      = vk::PipelineStageFlagBits::eColorAttachmentOutput;

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

    static void OnWindowResized(GLFWwindow* p_window, int p_width, int p_height);
public:
    //Initializes the base of vulkan
    virtual void Initialize();

    //Initializes the render type specific parts
    virtual void Prepare();

    virtual //Cleans up the mess vulkan makes
    void Cleanup();

    //Constructor
    VulkanRendererBase();

    //Destructor
    virtual ~VulkanRendererBase();

    //Halts until device is idle
    void DeviceWaitIdle();

    //Gives a pointer to the underlying GLFW window
    WrappedVulkanWindow * GetWindow();

    //Draws the frame with the current commandbuffers
    void RenderFrame();

protected:
    void InitializeGlfwWindow();

    void CreateInstance();

    void CreateDebugCallback();

    void CreateSurface();

    void SelectPhysicalDevice();

    void CreateLogicalDevice();

    void ConnectSwapchain();

    void SetupSwapchain();

    void CreateCommandPool();

    void CreateImageViews();

    void CreateCommandBuffers();

    void SetupDepthStencil();

    void SetupRenderPass();

    void CreatePipelineCache();

    void SetupFrameBuffer();

    //If the swapchain gets invalidated (for example during a window resize), this recreates all resources required
    void RebuildSwapchain();

    void DestroyCommandBuffers();

    void DestroyDepthStencil();

    void DestroySwapchain();

    void DestroyFrameBuffers();


    //Part of instance creation, gets the list of extensions that GLFW needs
    std::vector<const char *> GetRequiredExtensions();

    //Part of DebugCallback creation, checks if the validation layers in WrappedVulkanValidation are available
    bool CheckValidationLayerSupport();

    //Is used to check for specific components for ren
    virtual void GetEnabledFeatures();

    //Checks if a given physical device supports the required queue families
    bool IsDeviceSuitable(vk::PhysicalDevice p_device);

    //Completely dependent on implementation of the derived rendering system
    virtual void BuildCommandBuffers() = 0;

    virtual void Render() = 0;

    //Loads a shader though the helper function in VulkanHelpers, but it also saves a reference for easy deletion on cleanup.
    vk::PipelineShaderStageCreateInfo LoadShader(const std::string &p_fileName, vk::ShaderStageFlagBits p_shaderStage);

    //Contains the start and ending of a frame render call
    void PrepareFrame();

    void SubmitFrame();

    void CreateSemaphores();

    virtual void WindowResized(){};
};


#endif //VULKANENGINE_VULKANRENDERERBASE_HPP
