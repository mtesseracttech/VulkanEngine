//
// Created by MTesseracT on 19-9-2017.
//

#ifndef VULKANENGINE_GLFWWINDOW_HPP
#define VULKANENGINE_GLFWWINDOW_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GlfwWindow
{
public:
    GlfwWindow(std::string name, glm::ivec2 dims);
    ~GlfwWindow();
    GLFWwindow * GetWindow() {return m_window;};
private:
    GLFWwindow * m_window;
};


#endif //VULKANENGINE_GLFWWINDOW_HPP
