//
// Created by MTesseracT on 19-9-2017.
//

#include "GlfwWindow.hpp"

GlfwWindow::GlfwWindow(std::string name, glm::ivec2 dims)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(dims.x, dims.y, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    //glfwSetWindowSizeCallback(m_window, VulkanTriangle::OnWindowResized);
}

GlfwWindow::~GlfwWindow()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
