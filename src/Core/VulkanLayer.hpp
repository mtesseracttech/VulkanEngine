//
// Created by MTesseracT on 07/09/2017.
//

#ifndef VULKANENGINE_VULKANLAYER_HPP
#define VULKANENGINE_VULKANLAYER_HPP

//Vulkan
#include <vulkan/vulkan.h>

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

//Own Classes
#include "Model.hpp"
#include "../Utility/Constants.hpp"


//Constants
const int WIDTH = 800;
const int HEIGHT = 600;

#if defined(WIN32)
const std::vector<const char *> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};
#else
const std::vector<const char *> validationLayers = {
};
#endif

const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


//Debug/release dependent consts

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


//Data structs

struct QueueFamilyIndices
{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete()
    {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = static_cast<uint32_t>(offsetof(Vertex, pos));

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = static_cast<uint32_t>(offsetof(Vertex, color));

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = static_cast<uint32_t>(offsetof(Vertex, texCoord));

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const
    {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};


class VulkanLayer
{
public:
    void run()
    {
        InitializeGlfwWindow();
        InitializeVulkan();
        Loop();
        Cleanup();
    }

private:
    //GLFW Objects
    GLFWwindow *                    m_window;
    //Vulkan Instance
    VkInstance                      m_instance;
    //Vulkan Surface
    VkSurfaceKHR                    m_surface;
    //Debug
    VkDebugReportCallbackEXT        m_callback;
    //Devices
    VkPhysicalDevice                m_physicalDevice = VK_NULL_HANDLE;
    VkDevice                        m_device;
    //Queues
    VkQueue                         m_graphicsQueue;
    VkQueue                         m_presentQueue;
    //SwapChain Related
    VkSwapchainKHR                  m_swapChain;
    VkFormat                        m_swapChainImageFormat;
    VkExtent2D                      m_swapChainExtent;
    std::vector<VkImage>            m_swapChainImages;
    std::vector<VkImageView>        m_swapChainImageViews;
    std::vector<VkFramebuffer>      m_swapChainFramebuffers;
    //Render Pass Related
    VkRenderPass                    m_renderPass;
    //Pipeline Related
    VkDescriptorSetLayout           m_descriptorSetLayout;
    VkPipelineLayout                m_pipelineLayout;
    VkPipeline                      m_graphicsPipeline;
    //Buffer Related
    std::vector<Vertex>             m_vertices;
    std::vector<uint32_t>           m_indices;
    VkBuffer                        m_vertexBuffer;
    VkDeviceMemory                  m_vertexBufferMemory;
    VkBuffer                        m_indexBuffer;
    VkDeviceMemory                  m_indexBufferMemory;
    VkBuffer                        m_uniformBuffer;
    VkDeviceMemory                  m_uniformBufferMemory;
    //Command/Descriptor Pool
    VkCommandPool                   m_commandPool;
    std::vector<VkCommandBuffer>    m_commandBuffers;
    VkDescriptorPool                m_descriptorPool;
    VkDescriptorSet                 m_descriptorSet;
    //Texture Related
    VkImage                         m_textureImage;
    VkDeviceMemory                  m_textureImageMemory;
    VkImageView                     m_textureImageView;
    VkSampler                       m_textureSampler;
    //Depth Buffer
    VkImage                         m_depthImage;
    VkDeviceMemory                  m_depthImageMemory;
    VkImageView                     m_depthImageView;
    //Semaphores
    VkSemaphore                     m_imageAvailableSemaphore;
    VkSemaphore                     m_renderFinishedSemaphore;



    //Run Methods
    void InitializeGlfwWindow();
    void InitializeVulkan();
    void Loop();
    void Cleanup();
    //Vulkan Init Methods
    void CreateInstance();
    void SetupDebugCallback();
    void CreateSurface();
    void SelectPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapchain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateFramebuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSemaphores();
    //Extension Checking
    std::vector<const char*> GetRequiredExtensions();
    //bool CheckExtensions(const std::vector<VkExtensionProperties> p_vkExtensions, const char ** p_glfwReqExtensions, unsigned int p_glfwReqExtLength);
    //Validation Layer Checking
    bool CheckValidationLayerSupport();
    //Device Checking
    bool IsDeviceSuitable(VkPhysicalDevice p_device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice p_device);
    //Queue related
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice p_device);
    SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice p_device);
    //Swapchain
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &p_availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> p_availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &p_capabilities);
    void RecreateSwapchain();
    void CleanupSwapchain();
    static void OnWindowResized(GLFWwindow *p_window, int p_width, int p_height); //called to recreate the swapchain
    //Shader
    static std::vector<char> ReadFile(const std::string &p_filename);
    VkShaderModule CreateShaderModule(const std::vector<char> &p_code);
    //Main Loop Functions
    void UpdateUniformBuffer();
    void DrawFrame();
    //Pipeline related
    void CreateDescriptorSetLayout();
    void CreateDescriptorSet();
    //Command pool related
    void CreateDescriptorPool();
    VkCommandBuffer BeginSingleTimeCommands();
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
    //Buffer related
    void CreateBuffer(
            VkDeviceSize p_size,
            VkBufferUsageFlags p_usage,
            VkMemoryPropertyFlags p_properties,
            VkBuffer &p_buffer,
            VkDeviceMemory &p_bufferMemory);
    void CopyBuffer(
            VkBuffer p_srcBuffer,
            VkBuffer p_dstBuffer,
            VkDeviceSize p_size);
    uint32_t FindMemoryType(uint32_t p_typeFilter, VkMemoryPropertyFlags p_properties);
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void CreateUniformBuffer();
    //Texture Related
    void CreateTextureImage();
    void CreateTextureImageView();
    void CreateTextureSampler();
    VkImageView CreateImageView(VkImage p_image, VkFormat p_format, VkImageAspectFlags p_aspectFlags);
    void CreateImage(
            uint32_t p_width,
            uint32_t p_height,
            VkFormat p_format,
            VkImageTiling p_tiling,
            VkImageUsageFlags p_usage,
            VkMemoryPropertyFlags p_properties,
            VkImage & p_image,
            VkDeviceMemory & p_imageMemory);
    VkFormat FindSupportedFormat(
            const std::vector<VkFormat> & p_candidates,
            VkImageTiling p_tiling,
            VkFormatFeatureFlags p_features);
    void TransitionImageLayout(
            VkImage p_image,
            VkFormat p_format,
            VkImageLayout p_oldLayout,
            VkImageLayout p_newLayout);
    void CopyBufferToImage(
            VkBuffer p_buffer,
            VkImage p_image,
            uint32_t p_width,
            uint32_t p_height);
    //Depth Buffer Related
    void CreateDepthResources();
    VkFormat FindDepthFormat();
    bool HasStencilComponent(VkFormat p_format);
    //Model Related
    void LoadModel();
    //Debug Callbacks
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugReportFlagsEXT p_flags,
            VkDebugReportObjectTypeEXT p_objType,
            uint64_t p_obj,
            size_t p_location,
            int32_t p_code,
            const char* p_layerPrefix,
            const char* p_msg,
            void* p_userData);
};

#endif //VULKANENGINE_VULKANLAYER_HPP
