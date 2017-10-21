//
// Created by MTesseracT on 20-10-2017.
//

#ifndef VULKANENGINE_WRAPPEDVULKANVALIDATION_HPP
#define VULKANENGINE_WRAPPEDVULKANVALIDATION_HPP


#include <vector>
#include <iostream>
#include <vulkan/vulkan.hpp>
#include <string>
#include <sstream>

namespace mtvk
{
    namespace debug
    {
#if defined(WIN32)
        const std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
#else
        const std::vector<const char *> validationLayers = {/*TODO: Getting validation layers to work on Linux*/};
#endif

        const std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

#ifdef NDEBUG
        const bool EnableValidationLayers = false;
#else
        const bool EnableValidationLayers = true;
#endif
        VkBool32 messageCallback(
                VkDebugReportFlagsEXT p_flags,
                VkDebugReportObjectTypeEXT p_objectType,
                uint64_t p_sourceObject,
                size_t p_location,
                int32_t p_messageCode,
                const char *p_layerPrefix,
                const char *p_message,
                void *p_userData);



        void SetupDebugCallback(vk::Instance p_instance, vk::DebugReportFlagsEXT p_flags, vk::DebugReportCallbackEXT p_callBack);
    }

};


#endif //VULKANENGINE_WRAPPEDVULKANVALIDATION_HPP
