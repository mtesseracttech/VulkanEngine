//
// Created by MTesseracT on 12-2-2018.
//

#include "Display.hpp"

Display::Display()
{}

Display::~Display()
{
    Cleanup();
}

//Singleton C++11 Style
Display *Display::Get()
{
    static Display instance;
    return &instance;
}

void Display::Initialize()
{
    CreateWindow();
    CreateInstance();
    CreateDebugCallback();
    CreateSurface();
    SelectPhysicalDevice();
}

void Display::CreateWindow()
{
    m_window.Create(800, 600, "Loading Wolfsbane", false);
    glfwSetWindowUserPointer(m_window.GetGlfwWindow(), this);
    glfwSetWindowSizeCallback(m_window.GetGlfwWindow(), OnWindowResized);
}

void Display::CreateInstance()
{
    if (m_debug.GetEnableValidationLayers() && !m_debug.CheckValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers required, but not available!");
    }

    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName   = "Wolfsbane";
    appInfo.pEngineName        = "Wolfsbane Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = m_debug.GetRequiredExtensions();
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (m_debug.GetEnableValidationLayers())
    {
        createInfo.enabledLayerCount   = static_cast<uint32_t>(m_debug.GetValidationLayers().size());
        createInfo.ppEnabledLayerNames = m_debug.GetValidationLayers().data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    m_instance = vk::createInstance(createInfo);
}

void Display::OnWindowResized(GLFWwindow *p_window, int p_width, int p_height)
{
    auto display = reinterpret_cast<Display *>(glfwGetWindowUserPointer(p_window));
    //display->RebuildSwapchain();
    //display->WindowResized();
}

void Display::Cleanup()
{
    m_debug.Destroy();
    m_window.CleanupSurface();
    m_window.Destroy();
    m_instance.destroy();
}

void Display::CreateDebugCallback()
{
    m_debug.Create(m_instance);
}

void Display::SelectPhysicalDevice()
{
    std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices();

    if (devices.empty()) throw std::runtime_error("No devices found with Vulkan support");
    else
    {
        Logger::Log("Found the following Devices with Vulkan Support");
        int             devIndex = 0;
        for (const auto &device : devices)
        {
            vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
            std::stringstream            line;
            line << "Device [" << devIndex << "] : " << deviceProperties.deviceName;
            Logger::Log(line.str());
            line.str("");
            line << "Type: " << VulkanHelpers::GetDeviceTypeName(deviceProperties.deviceType);
            Logger::Log(line.str());
            line.str("");
            line << "API: " << (deviceProperties.apiVersion >> 22) << "."
                 << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion & 0xfff);
            Logger::Log(line.str());
            ++devIndex;
        }
    }

    //Ranking the suitable devices
    std::multimap<int, vk::PhysicalDevice> scoredDevices;

    for (const auto &device : devices)
    {
        scoredDevices.insert(std::make_pair(GetDeviceScore(device), device));
    }

    if (Logger::GetLogLevel() <= LogLevel::LevelInfo)
    {
        int devIndex = 0;
        Logger::Log("Scored the found devices as follows: ");
        for (const auto &device : scoredDevices)
        {
            vk::PhysicalDeviceProperties deviceProperties = device.second.getProperties();
            std::stringstream            line;
            line << "Device [" << devIndex << "] : " << deviceProperties.deviceName << " - Score: [" << device.first
                 << "]";
            Logger::Log(line.str());
            ++devIndex;
        }
    }


    //Creating a logical device out of the best entry
    if (scoredDevices.rbegin()->first > 0) m_device.Create(scoredDevices.rbegin()->second);
    else throw std::runtime_error("No device with a score of over 0 was found");
}


int Display::GetDeviceScore(vk::PhysicalDevice p_physicalDevice)
{
    vk::PhysicalDeviceProperties properties = p_physicalDevice.getProperties();
    vk::PhysicalDeviceFeatures   features   = p_physicalDevice.getFeatures();

    int score = 0;

    //If the queue families are not complete, using the device isn't going to happen
    if (!VulkanHelpers::FindQueueFamilies(p_physicalDevice, m_window.GetSurface()).IsComplete()) return 0;

    //I'd like to play with tesselation and geometry shaders, so it would be nice if they were supported
    if (!features.tessellationShader || !features.geometryShader)
    {
        Logger::Log("No tesselation or geometry shading capabilities, GPU not suitable");
        return 0;
    }

    // GPUS ranked based on desirability
    switch (properties.deviceType)
    {
        case vk::PhysicalDeviceType::eDiscreteGpu:
            score += 1000;
            break;
        case vk::PhysicalDeviceType::eIntegratedGpu:
            score += 800;
            break;
        case vk::PhysicalDeviceType::eCpu:
            score += 600;
            break;
        case vk::PhysicalDeviceType::eVirtualGpu:
            score += 400;
            break;
        case vk::PhysicalDeviceType::eOther:
            score += 200;
            break;
    }

    score += properties.limits.maxImageDimension2D;
    score += properties.limits.maxImageDimension3D;

    return score;
}

bool Display::IsDeviceSuitable(vk::PhysicalDevice p_device)
{
    vk::PhysicalDeviceFeatures features = p_device.getFeatures();

    return features.geometryShader && features.tessellationShader;
}

void Display::CreateSurface()
{
    m_window.CreateSurface(m_instance);
}

void Display::CreateLogicalDevice()
{
    vk::PhysicalDeviceFeatures deviceFeatures;

    m_device.CreateLogicalDevice(deviceFeatures, m_deviceExtensions);
}




