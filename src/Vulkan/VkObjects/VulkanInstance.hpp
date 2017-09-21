//
// Created by MTesseracT on 20-9-2017.
//

#ifndef VULKANENGINE_VULKANINSTANCE_HPP
#define VULKANENGINE_VULKANINSTANCE_HPP

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

class VulkanInstance
{
public:
    VulkanInstance(const bool p_enableValidationLayers, const std::vector<const char *>* p_validationLayers);

    virtual ~VulkanInstance();

    vk::Instance * GetInstance(){return &m_instance;};
private:
    bool CheckValidationLayerSupport();

    std::vector<const char *> GetRequiredExtensions();

    vk::Instance m_instance;

    const std::vector<const char *>* m_validationLayers;

    bool m_enableValidationLayers;

    void LogRequiredExtensions(std::vector<const char *> vector);
};


#endif //VULKANENGINE_VULKANINSTANCE_HPP
