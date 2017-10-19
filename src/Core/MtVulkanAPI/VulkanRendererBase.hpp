//
// Created by MTesseracT on 17-10-2017.
//

#ifndef VULKANENGINE_VULKANRENDERERBASE_HPP
#define VULKANENGINE_VULKANRENDERERBASE_HPP

//Vulkan
#include <vulkan/vulkan.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM Related
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/hash.hpp>

//STB
#include <stb_image.h>

//STL/C libs
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <array>
#include <chrono>
#include <unordered_map>

//Own Classes
#include "Utility/Constants.hpp"

class VulkanRendererBase
{
private:
    GLFWwindow * m_window;
    int m_windowHeight = 720;
    int m_windowWidth = 1080;
protected:
public:
    void InitializeGlfwWindow();
    void OnWindowResized(GLFWwindow * p_window,int p_width,int p_height);
    void OnKeyPressed(GLFWwindow * p_window, int p_key, int p_scancode, int p_action, int p_mods);
};


#endif //VULKANENGINE_VULKANRENDERERBASE_HPP
