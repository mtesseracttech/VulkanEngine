//
// Created by MTesseracT on 17-10-2017.
//

#include "VulkanRendererBase.hpp"

void VulkanRendererBase::InitializeGlfwWindow()
{
    m_window = new WrappedVulkanWindow(m_windowWidth, m_windowHeight, "Vulkan", false);
    /*Logger::Log("GLFW is initializing");
    if(glfwInit() == GLFW_TRUE)
    {
        Logger::Log("GLFW Initialized Successfully!");
    }
    else
    {
        throw std::runtime_error("GLFW Failed to initialize, terminating...");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Logger::Log("GLFW set to load without an API, so Vulkan can be used");
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Vulkan", (m_settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
    glfwSetWindowUserPointer(m_window, this);
     */
}

void VulkanRendererBase::Initialize()
{
    Logger::Log("Initializing the Renderer");
    InitializeGlfwWindow();
    CreateInstance();
    CreateDebugCallback();
    CreateSurface();
    SelectPhysicalDevice();
    CreateLogicalDevice();
}

void VulkanRendererBase::CreateInstance()
{
    m_settings.validation = mtvk::debug::EnableValidationLayers;

    if (m_settings.validation && !CheckValidationLayerSupport())
    {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    vk::ApplicationInfo applicationInfo;
    applicationInfo.pApplicationName = "";
    applicationInfo.pEngineName = "";
    applicationInfo.apiVersion = VK_API_VERSION_1_0;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    applicationInfo.engineVersion = VK_MAKE_VERSION(1,0,0);

    vk::InstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

    auto requiredExtensions = GetRequiredExtensions();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if(m_settings.validation)
    {
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mtvk::debug::validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = mtvk::debug::validationLayers.data();
    }
    else
    {
        instanceCreateInfo.enabledLayerCount = 0;
    }

    m_instance = vk::createInstance(instanceCreateInfo, nullptr);



    Logger::Log("Vulkan instance successfully created!");
}

bool VulkanRendererBase::CheckValidationLayerSupport()
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char *layerName : mtvk::debug::validationLayers)
    {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

std::vector<const char *> VulkanRendererBase::GetRequiredExtensions()
{
    std::vector<const char *> extensions;

    unsigned int glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (unsigned int i = 0; i < glfwExtensionCount; i++)
    {
        extensions.push_back(glfwExtensions[i]);
    }

    if (m_settings.validation)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void VulkanRendererBase::CreateDebugCallback()
{
    if(!m_settings.validation)
    {
        Logger::Log("Debug callback is not needed, not setting it up");
        return;
    }
    else
    {
        Logger::Log("Setting up debug callback");
    }

    vk::DebugReportFlagsEXT debugReportFlags = vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning;
    mtvk::debug::SetupDebugCallback(m_instance, debugReportFlags, nullptr);
}

void VulkanRendererBase::SelectPhysicalDevice()
{
    Logger::Log("Selecting a physical device to use");
    std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();

    if(physicalDevices.empty())
    {
        throw std::runtime_error("Could not find any physical devices to use");
    }

    Logger::Log("Found the following GPUs:");

    for(const auto& device : physicalDevices)
    {
        std::stringstream deviceLine;
        deviceLine << "- " << device.getProperties().deviceName << " (" << GetDeviceTypeName(
                device.getProperties().deviceType) << ")";
        Logger::Log(deviceLine.str());
    }

    //TODO: Add a device suitability check later on

    m_physicalDevice = physicalDevices[0];
    m_deviceProperties = m_physicalDevice.getProperties();
    m_deviceFeatures = m_physicalDevice.getFeatures();
    m_deviceMemoryProperties = m_physicalDevice.getMemoryProperties();

    GetEnabledFeatures();
}

std::string VulkanRendererBase::GetDeviceTypeName(vk::PhysicalDeviceType p_type)
{
    switch (p_type)
    {
        case vk::PhysicalDeviceType::eOther:         return "Other";
        case vk::PhysicalDeviceType::eIntegratedGpu: return "Integrated";
        case vk::PhysicalDeviceType::eDiscreteGpu:   return "Discrete";
        case vk::PhysicalDeviceType::eVirtualGpu:    return "Virtual";
        case vk::PhysicalDeviceType::eCpu:           return "CPU";
    }
}

void VulkanRendererBase::GetEnabledFeatures()
{
    //m_enabledFeatures = m_deviceFeatures;
    //Should be overridden if special functionality should be used
}

void VulkanRendererBase::CreateLogicalDevice()
{
    m_wrappedDevice = new WrappedVulkanDevice(m_physicalDevice);
    m_wrappedDevice->CreateLogicalDevice(m_enabledFeatures, m_enabledExtensions);
    m_logicalDevice = m_wrappedDevice->m_logicalDevice;

    m_graphicsQueue = m_logicalDevice.getQueue(m_wrappedDevice->m_queueFamilyIndices.graphics, 0);

    vk::Bool32 validDepthFormat = VulkanHelpers::GetSupportedDepthFormat(m_physicalDevice, &m_depthFormat);

    if(!validDepthFormat) throw std::runtime_error("Could not find a valid depth format");
}

void VulkanRendererBase::CreateSurface()
{
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(static_cast<VkInstance >(m_instance), m_window->m_window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    else{
        Logger::Log("Successfully created KHR Surface");
    }
    m_window->m_surface = static_cast<vk::SurfaceKHR>(surface);
}

void VulkanRendererBase::CreateSwapchain()
{
    m_swapchain.Create(m_wrappedDevice->m_physicalDevice, m_window);
}
