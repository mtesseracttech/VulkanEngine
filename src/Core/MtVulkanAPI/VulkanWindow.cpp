//
// Created by MTesseracT on 4-2-2018.
//

#include "VulkanWindow.hpp"
#include <GLFW/glfw3.h>

void VulkanWindow::Create(int windowWidth, int windowHeight, std::string windowTitle, bool fullscreen)
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



void VulkanWindow::CreateSurface(vk::Instance p_instance)
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(static_cast<VkInstance>(p_instance), m_window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
    else Logger::Log("Successfully created KHR Surface");

    m_surface = vk::SurfaceKHR(surface);
    m_instance = p_instance;
}

glm::vec2 VulkanWindow::GetCursorPos()
{
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return glm::vec2(x, y);
}

glm::ivec2 VulkanWindow::GetWindowSize()
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return glm::ivec2(width, height);
}

bool VulkanWindow::ShouldClose()
{
    return glfwWindowShouldClose(m_window) == GLFW_TRUE;
}

GLFWwindow *VulkanWindow::GetGlfwWindow()
{
    return m_window;
}

const vk::SurfaceKHR VulkanWindow::GetSurface()
{
    return m_surface;
}

void VulkanWindow::CleanupSurface()
{
    if(m_surface)m_instance.destroySurfaceKHR(m_surface);

    m_surface = nullptr;
    m_instance = nullptr;
}

void VulkanWindow::SetTitle(const std::string p_title)
{
    glfwSetWindowTitle(m_window, p_title.c_str());
}

float VulkanWindow::GetAspectRatio()
{
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return static_cast<float>(width) / static_cast<float>(height);
}

void VulkanWindow::Destroy()
{
    CleanupSurface();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}


