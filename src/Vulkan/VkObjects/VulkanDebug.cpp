//
// Created by MTesseracT on 21-9-2017.
//

#include "VulkanDebug.hpp"

VulkanDebug::VulkanDebug(bool p_enableValidationLayers, const std::vector<const char *>* p_validationLayers, VulkanInstance * p_instance) : m_validationLayers(p_validationLayers), m_instance(p_instance->GetInstance())
{
    if (p_enableValidationLayers)
    {
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = DebugCallback;

        if (CreateDebugReportCallbackEXT(*m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to set up Debug Callback!");
        }
    }
    else
    {
        m_callback = nullptr;
    }
}

VulkanDebug::~VulkanDebug()
{
    DestroyDebugReportCallbackEXT(*m_instance, m_callback, nullptr);
}

VkResult VulkanDebug::CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanDebug::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
                                   const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance,
                                                                            "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

VkBool32 VulkanDebug::DebugCallback(VkDebugReportFlagsEXT flags,
                                       VkDebugReportObjectTypeEXT objType,
                                       uint64_t obj,
                                       size_t location,
                                       int32_t code,
                                       const char *layerPrefix,
                                       const char *msg,
                                       void *userData)
{

    std::cerr << "validation layer: " << msg << std::endl;

    return VK_FALSE;
}