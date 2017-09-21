//
// Created by MTesseracT on 21-9-2017.
//

#ifndef VULKANENGINE_VULKANDEBUG_HPP
#define VULKANENGINE_VULKANDEBUG_HPP


#include "VulkanInstance.hpp"

class VulkanDebug
{
public:
    VulkanDebug(bool p_enableValidationLayers, const std::vector<const char *> *p_validationLayers,
                    VulkanInstance *p_instance);

    virtual ~VulkanDebug();
private:
    VkDebugReportCallbackEXT m_callback;
    vk::Instance * m_instance;


    const std::vector<const char *>* m_validationLayers;

    VkResult CreateDebugReportCallbackEXT(
        VkInstance instance,
        const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugReportCallbackEXT* pCallback
    );

    void DestroyDebugReportCallbackEXT(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks* pAllocator
    );

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugReportFlagsEXT flags,
            VkDebugReportObjectTypeEXT objType,
            uint64_t obj,
            size_t location,
            int32_t code,
            const char* layerPrefix,
            const char* msg,
            void* userData
    );
};


#endif //VULKANENGINE_VULKANDEBUG_HPP
