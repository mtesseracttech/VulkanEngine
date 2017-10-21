//
// Created by MTesseracT on 20-10-2017.
// Mostly replicated from SaschaWillems Vulkan Example Base
//

#include "WrappedVulkanValidation.hpp"

namespace mtvk
{
    namespace debug
    {
        PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = VK_NULL_HANDLE;
        PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = VK_NULL_HANDLE;
        PFN_vkDebugReportMessageEXT dbgBreakCallback = VK_NULL_HANDLE;

        VkDebugReportCallbackEXT msgCallback;

        void SetupDebugCallback(vk::Instance p_instance,
                                vk::DebugReportFlagsEXT p_flags,
                                vk::DebugReportCallbackEXT p_callback)
        {
            auto flags    = static_cast<VkDebugReportFlagsEXT>(p_flags);
            auto instance = static_cast<VkInstance>(p_instance);
            auto callback = static_cast<VkDebugReportCallbackEXT>(p_callback);

            CreateDebugReportCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(
                    p_instance, "vkCreateDebugReportCallbackEXT"));
            DestroyDebugReportCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(
                    p_instance, "vkDestroyDebugReportCallbackEXT"));
            dbgBreakCallback = reinterpret_cast<PFN_vkDebugReportMessageEXT>(vkGetInstanceProcAddr(p_instance,
                                                                                                   "vkDebugReportMessageEXT"));

            VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
            dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
            dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT) messageCallback;
            dbgCreateInfo.flags = flags;

            VkResult err = CreateDebugReportCallback(
                    instance,
                    &dbgCreateInfo,
                    nullptr,
                    (callback != VK_NULL_HANDLE) ? &callback : &msgCallback);
            assert(!err);
        }

        VkBool32 messageCallback(
                VkDebugReportFlagsEXT p_flags,
                VkDebugReportObjectTypeEXT p_objectType,
                uint64_t p_sourceObject,
                size_t p_location,
                int32_t p_messageCode,
                const char *p_layerPrefix,
                const char *p_message,
                void *p_userData)
        {

            std::string prefix("");

            if (p_flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)                prefix += "ERROR:";
            if (p_flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)              prefix += "WARNING:";
            if (p_flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)  prefix += "PERFORMANCE:";
            if (p_flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)          prefix += "INFO:";
            if (p_flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)                prefix += "DEBUG:";

            std::stringstream debugMessage;

            debugMessage << prefix << " [" << p_layerPrefix << "] Code " << p_messageCode << " : " << p_message;

            if (p_flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
            {
                std::cerr << debugMessage.str() << "\n";
            }
            else
            {
                std::cout << debugMessage.str() << "\n";
            }

            fflush(stdout);

            return VK_FALSE;
        }

        void FreeDebugCallback(vk::Instance p_instance)
        {
            if (msgCallback != VK_NULL_HANDLE)
            {
                DestroyDebugReportCallback(p_instance, msgCallback, nullptr);
            }
        }
    }
}

