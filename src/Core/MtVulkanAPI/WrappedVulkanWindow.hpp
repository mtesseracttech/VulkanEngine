//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
#define VULKANENGINE_VULKANWRAPPEDWINDOW_HPP

#include "Utility/Logger.hpp"
#include <exception>
#include <vulkan/vulkan.hpp>
#include "WrappedVulkanDevice.hpp"
#include <vector>
#include <glm/vec2.hpp>

struct GLFWwindow;

class WrappedVulkanWindow
{
private:
    GLFWwindow*     m_window;
    vk::Instance    m_instance;
    vk::SurfaceKHR  m_surface;
public:
    WrappedVulkanWindow(int windowWidth, int windowHeight, std::string windowTitle, bool fullscreen);

    void CreateSurface(vk::Instance p_instance);

    glm::vec2 GetCursorPos();

    glm::ivec2 GetWindowSize();

    bool ShouldClose();

    GLFWwindow * GetGlfwWindow();

    const vk::SurfaceKHR GetSurface();


    void CleanupSurface();

    void SetTitle(std::string p_title);

    float GetAspectRatio();
};

#endif //VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
