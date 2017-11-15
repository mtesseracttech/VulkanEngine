//
// Created by MTesseracT on 21-10-2017.
//

#ifndef VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
#define VULKANENGINE_VULKANWRAPPEDWINDOW_HPP

#include "Utility/Logger.hpp"
#include <GLFW/glfw3.h>
#include <exception>
#include <vulkan/vulkan.hpp>
#include "WrappedVulkanDevice.hpp"
#include <vector>

struct WrappedVulkanWindow
{
private:
    GLFWwindow*     m_window;
    vk::Instance    m_instance;
    vk::SurfaceKHR  m_surface;
public:
    WrappedVulkanWindow(int windowWidth, int windowHeight, std::string windowTitle, bool fullscreen)
    {
        Logger::Log("GLFW is initializing");
        if (glfwInit() == GLFW_TRUE)
        {
            Logger::Log("GLFW Initialized Successfully!");
        }
        else
        {
            throw std::runtime_error("GLFW Failed to initialize, terminating...");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        if(fullscreen)
        {
            int count;
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            windowWidth  = mode->width;
            windowHeight = mode->height;

            std::cout << "Primary monitor size: " << windowWidth << ", " << windowHeight << std::endl;
        }

        Logger::Log("GLFW set to load without an API, so Vulkan can be used");
        m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(),
                                    (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
    }

    void CreateSurface(vk::Instance p_instance)
    {
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(static_cast<VkInstance >(p_instance), m_window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
        else
        {
            Logger::Log("Successfully created KHR Surface");
        }
        m_surface = vk::SurfaceKHR(surface);//static_cast<vk::SurfaceKHR>(surface);
        m_instance = p_instance;
    }

    glm::vec2 GetCursorPos()
    {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        return glm::vec2(x, y);
    }

    glm::ivec2 GetWindowSize()
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return glm::ivec2(width, height);
    }

    bool ShouldClose()
    {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    GLFWwindow * GetGlfwWindow()
    {
        return m_window;
    }

    const vk::SurfaceKHR GetSurface()
    {
        return m_surface;
    }


    void CleanupSurface()
    {
        if(m_surface)m_instance.destroySurfaceKHR(m_surface);

        m_surface = nullptr;
    }

    void SetTitle(const std::string p_title)
    {
        glfwSetWindowTitle(m_window, p_title.c_str());
    }
};


#endif //VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
