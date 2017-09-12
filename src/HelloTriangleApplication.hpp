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

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication
{
public:
    void Run()
    {
        InitWindow();
        InitVulkan();
        MainLoop();
        Cleanup();
    }

private:
    GLFWwindow *m_window;
    VkInstance m_instance;

    void InitWindow()
    {
        //Initializes GLFW Library
        glfwInit();
        //Makes GLFW not create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        //Makes the GLFW window non-resizable
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        //Creates GLFW window
        m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    //Checks if all the extensions that GLFW asks for are actually supplied by vulkan
    bool CheckExtensions(const std::vector<VkExtensionProperties> p_vkExtensions,
                         const char ** p_glfwReqExtensions, unsigned int p_glfwReqExtLength)
    {
        for (int i = 0; i < p_glfwReqExtLength; ++i)
        {
            bool found = false;
            for (const auto &vkExtension : p_vkExtensions)
            {
                if(strcmp(p_glfwReqExtensions[i], vkExtension.extensionName) == 0)
                {
                    found = true;
                    break;
                }
            }
            if(!found){
                return false;
            }
        }
        return true;
    }

    //Checks if the requested validation layers are present
    bool CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }
        return true;
    }

    //Creates the Vulkan Instance and does some checks to make sure the environment is suitable
    void CreateInstance()
    {
        if (enableValidationLayers && !CheckValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        //Vulkan Application Info
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //Instance Creation Info
        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        //Initializing container and counter for GLFW extensions
        unsigned int glfwExtensionCount = 0;
        const char **glfwExtensions;

        //Getting the info from GLFW
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        //Feeding them into the creation info
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        //Creating the vkInstance
        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }
        else
        {
            std::cout << "The Vulkan Instance was created correctly" << std::endl;
        }

        //Checks GLFW to make sure Vulkan is supported
        std::cout << "Is Vulkan supported? " << (glfwVulkanSupported() == GLFW_TRUE ? "Yes" : "No") << std::endl;

        //Getting the extensions that are available from Vulkan
        unsigned int vkExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(vkExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, extensions.data());

        //Printing the extensions that vulkan is exposing
        std::cout << "Available Vulkan Extensions: " << vkExtensionCount << std::endl;
        for (const auto &extension : extensions)
        {
            std::cout << "\t" << extension.extensionName << std::endl;
        }

        //Printing the extensions that GLFW requires
        std::cout << "GLFW required extensions:" << std::endl;
        for (int i = 0; i < glfwExtensionCount; i++)
        {
            std::cout << "\t" << glfwExtensions[i] << std::endl;
        }

        CheckExtensions(extensions, glfwExtensions, glfwExtensionCount);
    }

    void InitVulkan()
    {
        CreateInstance();
    }

    void MainLoop()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            glfwPollEvents();
        }
    }

    void Cleanup()
    {
        //Destroys the vk instance
        vkDestroyInstance(m_instance, nullptr);
        //Destroys the window and frees up the memory
        glfwDestroyWindow(m_window);
        //Terminates the GLFW Library
        glfwTerminate();

    }
};

#endif //VULKANENGINE_HELLOTRIANGLEAPPLICATION_HPP
