//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
#define VULKANENGINE_VULKANWRAPPEDWINDOW_HPP

#include "Utility/Logger.hpp"
#include <exception>
#include <vulkan/vulkan.hpp>
#include "Core/MtVulkanAPI/Device/VulkanDevice.hpp"
#include <vector>
#include <glm/vec2.hpp>
#include <memory>

struct GLFWwindow;

class VulkanWindow
{
private:
    GLFWwindow     *m_window = nullptr;
    vk::Instance   m_instance = nullptr;
    vk::SurfaceKHR m_surface = nullptr;

    VulkanWindow() = default;

    ~VulkanWindow() = default;
public:


    void Create(int windowWidth, int windowHeight, std::string windowTitle, bool fullscreen);

    void Destroy();

    void CreateSurface(vk::Instance p_instance);

    glm::vec2 GetCursorPos();

    glm::ivec2 GetWindowSize();

    bool ShouldClose();

    GLFWwindow *GetGlfwWindow();

    const vk::SurfaceKHR GetSurface();


    void CleanupSurface();

    void SetTitle(std::string p_title);

    float GetAspectRatio();
};

#endif //VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
