//
// Created by MTesseracT on 17-10-2017.
//

#include "VulkanRendererBase.hpp"
#include "WrappedVulkanValidation.hpp"

void VulkanRendererBase::InitializeGlfwWindow()
{
    Logger::Log("GLFW is initializing");
    if(glfwInit() == GLFW_TRUE)
    {
        Logger::Log("GLFW Initialized Successfully!");
    }
    else
    {
        throw std::runtime_error("GLFW Failed to initialize, terminating...");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Logger::Log("GLFW set to load without an API, so Vulkan can be used");
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Vulkan", (m_settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
    glfwSetWindowUserPointer(m_window, this);
}

void VulkanRendererBase::Initialize()
{
    Logger::Log("Initializing the Renderer");
    InitializeGlfwWindow();
    CreateInstance();
    CreateDebugCallback();
}

void VulkanRendererBase::CreateInstance()
{
    m_settings.validation = mtvk::debug::EnableValidationLayers;

    if (m_settings.validation && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    vk::ApplicationInfo applicationInfo;
    applicationInfo.pApplicationName = "";
    applicationInfo.pEngineName = "";
    applicationInfo.apiVersion = VK_API_VERSION_1_0;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    applicationInfo.engineVersion = VK_MAKE_VERSION(1,0,0);

    vk::InstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

    auto requiredExtensions = GetRequiredExtensions();
    instanceCreateInfo.enabledExtensionCount = requiredExtensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if(m_settings.validation)
    {
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mtvk::debug::validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = mtvk::debug::validationLayers.data();
    }
    else
    {
        instanceCreateInfo.enabledLayerCount = 0;
    }

    m_instance = vk::createInstance(instanceCreateInfo, nullptr);

    Logger::Log("Vulkan instance successfully created!");
}

bool VulkanRendererBase::CheckValidationLayerSupport()
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : mtvk::debug::validationLayers)
    {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers)
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

std::vector<const char *> VulkanRendererBase::GetRequiredExtensions()
{
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    if (m_settings.validation)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void VulkanRendererBase::CreateDebugCallback()
{
    if(!m_settings.validation) return;

    vk::DebugReportFlagsEXT debugReportFlags = vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning;


}
