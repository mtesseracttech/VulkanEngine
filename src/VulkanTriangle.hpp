//
// Created by MTesseracT on 07/09/2017.
//

#ifndef VULKANENGINE_VULKANTRIANGLE_HPP
#define VULKANENGINE_VULKANTRIANGLE_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <array>

const int WIDTH = 800;
const int HEIGHT = 600;

#if defined(_WIN32)
const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};
#elif defined(__linux__)
const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};
#else //No validation layers available
const std::vector<const char*> validationLayers;
#endif


const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#if defined(NDEBUG) || defined(__APPLE__) //Because MoltenVK does not support validation layers yet
const bool enableValidationLayers = false;
const bool displayDebugInfo = false;
#else
const bool enableValidationLayers = true;
const bool displayDebugInfo = true;
#endif

VkResult CreateDebugReportCallbackEXT(VkInstance instance,
                                      const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugReportCallbackEXT* pCallback);

void DestroyDebugReportCallbackEXT(VkInstance instance,
                                   VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks* pAllocator);


struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = static_cast<uint32_t>(offsetof(Vertex, pos));

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = static_cast<uint32_t >(offsetof(Vertex, color));

        return attributeDescriptions;
    }
};

const std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
};

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete() {
        return (graphicsFamily >= 0 && presentFamily >= 0);
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};



class VulkanTriangle
{
public:
    void Run();

private:
    //GLFW
    GLFWwindow * m_window;

    //Other
    VkInstance m_instance;
    VkDebugReportCallbackEXT m_callback;
    VkSurfaceKHR m_surface;

    //Devices
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;

    //Queues
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    //Swap Chain Related
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;

    //Render Pass Related
    VkRenderPass m_renderPass;

    //Pipeline related
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    //Buffer related
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;

    //Framebuffers
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    //Command pool
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    //Semaphores
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;

    //Run methods
    void InitWindow();
    void InitVulkan();
    void MainLoop();
    void Cleanup();

    //Vulkan init methods
    void CreateInstance();
    void SetupDebugCallback();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSemaphores();

    //Extension Checking
    bool CheckExtensions(const std::vector<VkExtensionProperties> p_vkExtensions, const char ** p_glfwReqExtensions, unsigned int p_glfwReqExtLength);
    std::vector<const char*> GetRequiredExtensions();

    //Validation Layer Checking
    bool CheckValidationLayerSupport();

    //Device Checking
    bool IsDeviceSuitable(VkPhysicalDevice p_device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice p_device);

    //Queue related methods
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice p_device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice p_device);

    //Swap Chain related methods
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& p_availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> p_availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& p_capabilities);
    void CleanupSwapChain();
    void RecreateSwapChain();

    //Shader related
    VkShaderModule CreateShaderModule(const std::vector<char> &code);

    //Main Loop Functions
    void DrawFrame();

    //GLFW Related
    static void OnWindowResized(GLFWwindow* window, int width, int height);

    //Graphics Pipeline Related
    void CreateVertexBuffer();
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    //Debug Callbacks
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objType,
            uint64_t obj,
            size_t location,
            int32_t code,
            const char* layerPrefix,
            const char* msg,
            void* userData);
};

#endif //VULKANENGINE_VULKANTRIANGLE_HPP
