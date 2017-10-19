//
// Created by MTesseracT on 17-10-2017.
//

#include "VulkanRendererBase.hpp"

void VulkanRendererBase::InitializeGlfwWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(m_window, this);
    //glfwSetWindowSizeCallback(m_window, OnWindowResized); //TODO: setting up the GLFW calls
    //glfwSetKeyCallback(m_window, OnKeyPressed);
}
