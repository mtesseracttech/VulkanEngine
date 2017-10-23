//
// Created by MTesseracT on 17-10-2017.
//

#include "VulkanRendererBase.hpp"

void VulkanRendererBase::InitializeGlfwWindow()
{
    m_window = new WrappedVulkanWindow(m_windowWidth, m_windowHeight, "Vulkan", false);
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
    CreateSwapchain();
    CreateImageViews();
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
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    vk::InstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

    auto requiredExtensions = GetRequiredExtensions();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if (m_settings.validation)
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
    if (!m_settings.validation)
    {
        Logger::Log("Debug callback is not needed, not setting it up");
        return;
    }
    else
    {
        Logger::Log("Setting up debug callback");
    }

    vk::DebugReportFlagsEXT debugReportFlags =
            vk::DebugReportFlagBitsEXT::eDebug | vk::DebugReportFlagBitsEXT::eWarning;
    mtvk::debug::SetupDebugCallback(m_instance, debugReportFlags, nullptr);
}

void VulkanRendererBase::SelectPhysicalDevice()
{
    Logger::Log("Selecting a physical device to use");
    std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();

    if (physicalDevices.empty())
    {
        throw std::runtime_error("Could not find any physical devices to use");
    }


    if (physicalDevices.empty())
    {
        std::cerr << "No Vulkan devices found!" << std::endl;
    }
    else
    {
        Logger::Log("Found the following GPUs:");
        int devNumber = 0;
        for (const auto &device : physicalDevices)
        {
            vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
            std::stringstream line;
            line << "Device [" << devNumber << "] : " << deviceProperties.deviceName;
            Logger::Log(line.str());
            line.str("");
            line << "Type: " << VulkanHelpers::GetDeviceTypeName(deviceProperties.deviceType);
            Logger::Log(line.str());
            line.str("");
            line << "API: " << (deviceProperties.apiVersion >> 22) << "."
                 << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "." << (deviceProperties.apiVersion & 0xfff);
            Logger::Log(line.str());
            ++devNumber;
        }
    }

    for (const auto &device : physicalDevices)
    {
        if (IsDeviceSuitable(device))
        {
            m_physicalDevice = device;
            std::stringstream deviceString;
            deviceString << "Selected Device: " << m_physicalDevice.getProperties().deviceName;
            Logger::Log(deviceString.str());
            break;
        }
    }

    m_deviceProperties = m_physicalDevice.getProperties();
    m_deviceFeatures = m_physicalDevice.getFeatures();
    m_deviceMemoryProperties = m_physicalDevice.getMemoryProperties();

    GetEnabledFeatures();
}

bool VulkanRendererBase::IsDeviceSuitable(vk::PhysicalDevice p_device)
{
    QueueFamilyIndices indices = VulkanHelpers::FindQueueFamilies(p_device, m_window->m_surface);

    bool extensionsSupported = VulkanHelpers::CheckDeviceExtensionSupport(p_device, m_deviceExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = VulkanHelpers::QuerySwapChainSupport(p_device, m_window->m_surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    vk::PhysicalDeviceFeatures supportedFeatures = p_device.getFeatures();

    return indices.IsComplete() && extensionsSupported && supportedFeatures.samplerAnisotropy && swapChainAdequate;
}

void VulkanRendererBase::GetEnabledFeatures()
{
    //m_enabledFeatures = m_deviceFeatures;
    //Should be overridden if special functionality should be used
}

void VulkanRendererBase::CreateLogicalDevice()
{
    Logger::Log("Creating logical device");
    m_wrappedDevice = new WrappedVulkanDevice(m_physicalDevice);
    m_wrappedDevice->CreateLogicalDevice(m_enabledFeatures, m_deviceExtensions);
    m_logicalDevice = m_wrappedDevice->m_logicalDevice;

    m_graphicsQueue = m_logicalDevice.getQueue(m_wrappedDevice->m_queueFamilyIndices.graphics, 0);

    vk::Bool32 validDepthFormat = VulkanHelpers::GetSupportedDepthFormat(m_physicalDevice, &m_depthFormat);

    if (!validDepthFormat) throw std::runtime_error("Could not find a valid depth format");
}

void VulkanRendererBase::CreateSurface()
{
    m_window->CreateSurface(m_instance);
}

void VulkanRendererBase::CreateSwapchain()
{
    m_swapchain.Connect(m_wrappedDevice->m_physicalDevice, m_wrappedDevice->m_logicalDevice, m_window);
    m_swapchain.Create();
}

void VulkanRendererBase::CreateImageViews()
{
    m_swapchain.CreateImageViews();
}

void VulkanRendererBase::CreateRenderPass()
{
    vk::Format depthFormat;
    VulkanHelpers::GetSupportedDepthFormat(m_physicalDevice, &depthFormat);

    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = m_swapchain.GetSwapchainImageFormat();
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    vk::AttachmentDescription depthAttachment;
    depthAttachment.format = depthFormat;
    depthAttachment.samples = vk::SampleCountFlagBits::e1;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentReference colorAttachmentReference;
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentReference depthAttachmentReference;
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::SubpassDescription subpassDescription;
    subpassDescription.pipelineBindPoint = vk::PipelineBindPoint ::eGraphics;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

    vk::SubpassDependency subpassDependency;
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = vk::PipelineStageFlagBits ::eColorAttachmentOutput;
    subpassDependency.srcAccessMask = static_cast<vk::AccessFlagBits>(0);
    subpassDependency.dstStageMask = vk::PipelineStageFlagBits ::eColorAttachmentOutput;
    subpassDependency.dstAccessMask = vk::AccessFlagBits ::eColorAttachmentRead | vk::AccessFlagBits ::eColorAttachmentWrite;

    std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    vk::RenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.attachmentCount = attachments.size();
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    m_renderPass = m_logicalDevice.createRenderPass(renderPassCreateInfo);
}