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
    GLFWwindow *m_window;
    vk::SurfaceKHR m_surface;
public:

    GLFWwindow * GetWindow()
    {
        return m_window;
    }

    const vk::SurfaceKHR GetSurface()
    {
        return m_surface;
    }

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
        Logger::Log("GLFW set to load without an API, so Vulkan can be used");
        m_window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(),
                                    (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
        glfwSetWindowUserPointer(m_window, this);

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
        m_surface = static_cast<vk::SurfaceKHR>(surface);
    }
};


#endif //VULKANENGINE_VULKANWRAPPEDWINDOW_HPP
