//
// Created by MTesseracT on 17-10-2017.
//

#ifndef VULKANENGINE_VULKANRENDERERBASE_HPP
#define VULKANENGINE_VULKANRENDERERBASE_HPP

//Vulkan
#include <vulkan/vulkan.hpp>

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
#include <Utility/Logger.hpp>

class VulkanRendererBase
{
public:
    //Entry point
    void Initialize();
    void InitializeGlfwWindow();

    struct RendererSettings
    {
        bool validation = false;
        bool fullscreen = false;
        bool vsync = false;
    } m_settings;

protected:
private:
    GLFWwindow * m_window;
    int m_windowHeight = 720;
    int m_windowWidth = 1080;

    vk::Instance m_instance = nullptr;

    void CreateInstance();
    bool CheckValidationLayerSupport();
    std::vector<const char *> GetRequiredExtensions();

    void CreateDebugCallback();
};


#endif //VULKANENGINE_VULKANRENDERERBASE_HPP
