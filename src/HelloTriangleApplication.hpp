//
// Created by MTesseracT on 07/09/2017.
//

#ifndef VULKANENGINE_HELLOTRIANGLEAPPLICATION_HPP
#define VULKANENGINE_HELLOTRIANGLEAPPLICATION_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>

const int WIDTH = 800;
const int HEIGHT = 600;


const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};



#ifdef NDEBUG
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



class HelloTriangleApplication
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
    void CreateGraphicsPipeline();

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

#endif //VULKANENGINE_HELLOTRIANGLEAPPLICATION_HPP
