//
// Created by MTesseracT on 07/09/2017.
//

#ifndef VULKANENGINE_VULKANTRIANGLE_HPP
#define VULKANENGINE_VULKANTRIANGLE_HPP

//Vulkan
#include <vulkan/vulkan.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM Related
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

//Custom Classes
#include "Utility/ShaderUtils.hpp"


//Consts/temp magic numbers

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


//Platform dependent consts

#if defined(_WIN32)
const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};
#elif defined(__linux__)
const std::vector<const char*> validationLayers = {
};
#else //No validation layers available
const std::vector<const char*> validationLayers;
#endif


//Debug/release dependent consts

#if defined(NDEBUG)
const bool enableValidationLayers = false;
const bool displayDebugInfo = false;
#else
const bool enableValidationLayers = true;
const bool displayDebugInfo = true;
#endif


//Globally accessible methods

VkResult CreateDebugReportCallbackEXT(VkInstance instance,
                                      const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugReportCallbackEXT* pCallback);

void DestroyDebugReportCallbackEXT(VkInstance instance,
                                   VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks* pAllocator);


//Data structs

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

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
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


//Temp structs consts

const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
};

const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
};


class VulkanTriangle
{
public:
    void Run();

private:
    //GlfwObjects
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
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    //Buffer related
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;

    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;

    VkBuffer m_uniformBuffer;
    VkDeviceMemory m_uniformBufferMemory;

    //Texture Creation
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;

    //Framebuffers
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    //Command/Descriptor pool
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
    VkDescriptorPool m_descriptorPool;
    VkDescriptorSet m_descriptorSet;

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
    void UpdateUniformBuffer();
    void DrawFrame();

    //Glfw Related
    static void OnWindowResized(GLFWwindow* window, int width, int height);

    //Pipeline related
    void CreateDescriptorSetLayout();
    void CreateDescriptorSet();

    //Command Pool related
    void CreateDescriptorPool();

    //Buffer Related
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffer();
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
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

    void CreateTextureImage();
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    void TransitionImageLayout(VkImage p_image, VkFormat p_format, VkImageLayout p_oldLayout, VkImageLayout p_newLayout);

    void CopyBufferToImage(VkBuffer p_buffer, VkImage p_image, uint32_t p_width, uint32_t p_height);
};

#endif //VULKANENGINE_VULKANTRIANGLE_HPP
