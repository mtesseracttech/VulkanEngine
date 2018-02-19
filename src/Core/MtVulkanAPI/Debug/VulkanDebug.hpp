//
// Created by MTesseracT on 12-2-2018.
//

#ifndef VULKANENGINE_VULKANDEBUG_HPP
#define VULKANENGINE_VULKANDEBUG_HPP


#include <vector>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <map>

class VulkanDebug
{
#ifdef NDEBUG
    static const bool m_enableValidationLayers = false;
#else
    static const bool m_enableValidationLayers = true;
#endif

private:
#ifdef _WIN32
    const std::vector<const char *> m_validationLayers = {
            "VK_LAYER_LUNARG_standard_validation"
    };
#else
    const std::vector<const char *> m_validationLayers = {};
#endif

    vk::DebugReportCallbackEXT m_callback;
    vk::Instance               m_instance;

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugReportFlagsEXT p_flags,
            VkDebugReportObjectTypeEXT p_objType,
            uint64_t p_obj,
            size_t p_location,
            int32_t p_code,
            const char *p_layerPrefix,
            const char *p_msg,
            void *p_userData);

    inline void PrintDebug(vk::DebugReportFlagsEXT p_flags,
                           vk::DebugReportObjectTypeEXT p_typeFlags,
                           int32_t p_code,
                           const char *p_layerPrefix,
                           const char *p_message);

    vk::Result CreateDebugReportCallbackEXT(vk::Instance p_instance,
                                            const vk::DebugReportCallbackCreateInfoEXT *p_createInfo,
                                            const vk::AllocationCallbacks *p_allocator,
                                            vk::DebugReportCallbackEXT *p_callback);

    void DestroyDebugReportCallbackEXT(vk::Instance p_instance,
                                       vk::DebugReportCallbackEXT p_callback,
                                       const vk::AllocationCallbacks *p_allocator);

public:
    VulkanDebug() = default;
    ~VulkanDebug() = default;

    bool CheckValidationLayerSupport();

    std::vector<const char *> GetRequiredExtensions();

    bool GetEnableValidationLayers();

    const std::vector<const char *> GetValidationLayers();

    void Create(vk::Instance p_instance);

    void Destroy();

};


#endif //VULKANENGINE_VULKANDEBUG_HPP
