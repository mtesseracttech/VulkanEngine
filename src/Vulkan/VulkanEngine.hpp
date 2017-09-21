//
// Created by MTesseracT on 19-9-2017.
//

#ifndef VULKANENGINE_VULKANGLFWINSTANCE_HPP
#define VULKANENGINE_VULKANGLFWINSTANCE_HPP

#include <vulkan/vulkan.hpp>
#include "GLFW/GlfwWindow.hpp"
#include "VkObjects/VulkanInstance.hpp"
#include "VkObjects/VulkanDebug.hpp"


class VulkanEngine
{
public:
    void Run();
protected:
private:
    //GLFW
    GlfwWindow      * m_window   = nullptr;
    VulkanInstance  * m_instance = nullptr;
    VulkanDebug     * m_debug    = nullptr;

    void InitializeWindow();

    void InitializeVulkan();

    void Cleanup();

    void MainLoop();

#if NDEBUG
    const bool m_enableValidationLayers = false;
#else
    const bool m_enableValidationLayers = true;
#endif

    const std::vector<const char*> m_validationLayers = {
            "VK_LAYER_LUNARG_standard_validation"
    };

};


#endif //VULKANENGINE_VULKANGLFWINSTANCE_HPP
