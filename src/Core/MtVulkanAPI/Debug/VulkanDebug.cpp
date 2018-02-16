//
// Created by MTesseracT on 12-2-2018.
//

#include <GLFW/glfw3.h>
#include <sstream>
#include "VulkanDebug.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebug::DebugCallback(VkDebugReportFlagsEXT p_flags,
                                                          VkDebugReportObjectTypeEXT p_objType,
                                                          uint64_t p_obj,
                                                          size_t p_location,
                                                          int32_t p_code,
                                                          const char *p_layerPrefix,
                                                          const char *p_msg,
                                                          void *p_userData)
{
    auto flags   = vk::DebugReportFlagBitsEXT(p_flags);
    auto objType = vk::DebugReportObjectTypeEXT(p_objType);

    auto debugCaller = reinterpret_cast<VulkanDebug *>(p_userData);

    debugCaller->PrintDebug(flags, objType, p_code, p_layerPrefix, p_msg);

    return VK_FALSE;
}

bool VulkanDebug::CheckValidationLayerSupport()
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : m_validationLayers)
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

        if (!layerFound) return false;
    }

    return true;
}

std::vector<const char *> VulkanDebug::GetRequiredExtensions()
{
    uint32_t   glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (GetEnableValidationLayers()) extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    return extensions;
}

bool VulkanDebug::GetEnableValidationLayers()
{
    return m_enableValidationLayers;
}

const std::vector<const char *> VulkanDebug::GetValidationLayers()
{
    return m_validationLayers;
}

void VulkanDebug::Create(vk::Instance p_instance)
{
    assert(p_instance);

    m_instance = p_instance;

    if (!GetEnableValidationLayers()) return;

    vk::DebugReportCallbackCreateInfoEXT callbackCreateInfo;
    callbackCreateInfo.flags       = vk::DebugReportFlagBitsEXT ::eWarning |
            vk::DebugReportFlagBitsEXT::ePerformanceWarning |
            vk::DebugReportFlagBitsEXT::eError |
            vk::DebugReportFlagBitsEXT::eDebug;
    callbackCreateInfo.pfnCallback = DebugCallback;
    callbackCreateInfo.pUserData   = this;

    if (CreateDebugReportCallbackEXT(p_instance, &callbackCreateInfo, nullptr, &m_callback) != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to set up debug callback!");
    }

}

void VulkanDebug::PrintDebug(vk::DebugReportFlagsEXT p_reportflags,
                             vk::DebugReportObjectTypeEXT p_objectFlags,
                             int32_t p_code,
                             const char *p_layerPrefix,
                             const char *p_message)
{
    if (p_reportflags & vk::DebugReportFlagBitsEXT::eError)
    {
        std::cerr << "[VK ERROR][" << p_layerPrefix << "] " << p_code << " - " << p_message
                  << std::endl;
    }
    else if (p_reportflags & vk::DebugReportFlagBitsEXT::eWarning)
    {
        std::cerr << "[VK WARNING][" << p_layerPrefix << "] " << p_code << " - " << p_message
                  << std::endl;
    }
    else if (p_reportflags & vk::DebugReportFlagBitsEXT::ePerformanceWarning)
    {
        std::cerr << "[VK PERFORMANCE][" << p_layerPrefix << "] " << p_code << " - " << p_message
                  << std::endl;
    }
    else if (p_reportflags & vk::DebugReportFlagBitsEXT::eInformation)
    {
        std::cout << "[VK INFO][" << p_layerPrefix << "] " << p_code << " - " << p_message
                  << std::endl;
    }
    else if (p_reportflags & vk::DebugReportFlagBitsEXT::eDebug)
    {
        std::cout << "[VK DEBUG][" << p_layerPrefix << "] " << p_code << " - " << p_message
                  << std::endl;
    }
}

vk::Result VulkanDebug::CreateDebugReportCallbackEXT(vk::Instance p_instance,
                                                     const vk::DebugReportCallbackCreateInfoEXT *p_createInfo,
                                                     const vk::AllocationCallbacks *p_allocator,
                                                     vk::DebugReportCallbackEXT *p_callback)
{
    auto instance   = static_cast<VkInstance>(p_instance);
    auto createInfo = reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT *>(p_createInfo);
    auto allocator  = reinterpret_cast<const VkAllocationCallbacks *>(p_allocator);
    auto callback   = reinterpret_cast<VkDebugReportCallbackEXT *>(p_callback);

    auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(p_instance.getProcAddr("vkCreateDebugReportCallbackEXT"));

    //auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(p_instance, "vkCreateDebugReportCallbackEXT");

    if (func) return vk::Result(func(instance, createInfo, allocator, callback));

    return vk::Result::eErrorExtensionNotPresent;
}

void VulkanDebug::DestroyDebugReportCallbackEXT(vk::Instance p_instance,
                                                vk::DebugReportCallbackEXT p_callback,
                                                const vk::AllocationCallbacks *p_allocator)
{
    auto instance   = static_cast<VkInstance>(p_instance);
    auto allocator  = reinterpret_cast<const VkAllocationCallbacks *>(p_allocator);
    auto callback   = static_cast<VkDebugReportCallbackEXT>(p_callback);

    auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(p_instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));

    //auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

    if (func) func(instance, callback, allocator);
}

void VulkanDebug::Destroy()
{
    DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
}

