//
// Created by MTesseracT on 20-9-2017.
//

#include "VulkanInstance.hpp"
#include "../../Utility/Logger.hpp"
#include "../VulkanEngine.hpp"
#include "../../VulkanTriangle.hpp"
#include <iostream>

VulkanInstance::VulkanInstance(const bool p_enableValidationLayers, const std::vector<const char *>* p_validationLayers)
        : m_enableValidationLayers(p_enableValidationLayers), m_validationLayers(p_validationLayers)
{
    if (m_enableValidationLayers && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    vk::ApplicationInfo applicationInfo;
    applicationInfo.pApplicationName = "Test Instance";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    applicationInfo.pEngineName = "No Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    auto extensions = GetRequiredExtensions();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>((*p_validationLayers).size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    LogRequiredExtensions(extensions);

    if (m_enableValidationLayers)
    {
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>((*p_validationLayers).size());
        instanceCreateInfo.ppEnabledLayerNames = (*p_validationLayers).data();
    }
    else
    {
        instanceCreateInfo.enabledLayerCount = 0;
    }

    if(vk::createInstance(&instanceCreateInfo, nullptr, &m_instance) != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create instance!");
    }
    else
    {
        Logger::Log("Successfully Created Instance!", LogType::LogDebug);
    }
}

bool VulkanInstance::CheckValidationLayerSupport()
{
    uint32_t layerCount;

    vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<vk::LayerProperties> availableLayers(layerCount);
    vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : *m_validationLayers)
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

std::vector<const char *> VulkanInstance::GetRequiredExtensions()
{
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    if (m_enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
    return extensions;
}

void VulkanInstance::LogRequiredExtensions(std::vector<const char *> extensions)
{
    Logger::Log("GLFW Required Extensions: ", LogType::LogDebug);
    for (auto extension : extensions)
    {
        std::string extensionLine = "\t";
        Logger::Log(extensionLine + extension, LogType::LogDebug);
    }
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(m_instance, nullptr);
}