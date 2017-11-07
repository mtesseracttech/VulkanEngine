//
// Created by MTesseracT on 17-10-2017.
//

#include "VulkanRendererBase.hpp"
#include "WrappedVulkanDevice.hpp"

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
    ConnectSwapchain();
    CreateSemaphores();
}

void VulkanRendererBase::Prepare()
{
    SetupSwapchain();
    CreateCommandPool();
    CreateImageViews();
    CreateCommandBuffers();
    SetupDepthStencil();
    SetupRenderPass();
    CreatePipelineCache();
    SetupFrameBuffer();
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
    QueueFamilyIndices indices = VulkanHelpers::FindQueueFamilies(p_device, m_window->GetSurface());

    bool extensionsSupported = VulkanHelpers::CheckDeviceExtensionSupport(p_device, m_deviceExtensions);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = VulkanHelpers::QuerySwapChainSupport(p_device, m_window->GetSurface());
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

void VulkanRendererBase::ConnectSwapchain()
{
    m_swapchain.Connect(m_wrappedDevice->m_physicalDevice, m_wrappedDevice->m_logicalDevice, m_window);
}

void VulkanRendererBase::CreateImageViews()
{
    m_swapchain.CreateImageViews();
}

void VulkanRendererBase::CreateCommandPool()
{
    Logger::Log("Creating Command Pool");
    vk::CommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.queueFamilyIndex = m_wrappedDevice->m_queueFamilyIndices.graphics;

    m_commandPool = m_logicalDevice.createCommandPool(commandPoolCreateInfo);
}

void VulkanRendererBase::SetupSwapchain()
{
    m_swapchain.Create();
}

void VulkanRendererBase::CreateCommandBuffers()
{
    Logger::Log("Creating command buffers");
    m_drawCommandBuffers.resize(m_swapchain.GetImageViewCount());

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
    commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(m_drawCommandBuffers.size());

    m_logicalDevice.allocateCommandBuffers(&commandBufferAllocateInfo, m_drawCommandBuffers.data());
}

void VulkanRendererBase::SetupDepthStencil()
{
    Logger::Log("Setting up depth stencil");
    vk::ImageCreateInfo image;
    image.pNext = nullptr;
    image.imageType = vk::ImageType::e2D;
    image.format = m_depthFormat;
    image.extent.width = m_swapchain.GetExtent().width;
    image.extent.height = m_swapchain.GetExtent().height;
    image.extent.depth = 1;
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = vk::SampleCountFlagBits::e1;
    image.tiling = vk::ImageTiling::eOptimal;
    image.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransferSrc;
    image.flags = static_cast<vk::ImageCreateFlagBits>(0);

    m_depthStencil.image = m_logicalDevice.createImage(image);

    vk::MemoryRequirements memoryRequirements = m_logicalDevice.getImageMemoryRequirements(m_depthStencil.image);

    vk::MemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.pNext = nullptr;
    memoryAllocateInfo.allocationSize = 0;
    memoryAllocateInfo.memoryTypeIndex = 0;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = m_wrappedDevice->GetMemoryType(memoryRequirements.memoryTypeBits,
                                                                        vk::MemoryPropertyFlagBits::eDeviceLocal);
    m_depthStencil.memory = m_logicalDevice.allocateMemory(memoryAllocateInfo);
    m_logicalDevice.bindImageMemory(m_depthStencil.image, m_depthStencil.memory, 0);

    vk::ImageViewCreateInfo depthStencilView;
    depthStencilView.pNext = nullptr;
    depthStencilView.viewType = vk::ImageViewType::e2D;
    depthStencilView.format = m_depthFormat;
    depthStencilView.flags = static_cast<vk::ImageViewCreateFlagBits >(0);
    depthStencilView.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = m_depthStencil.image;

    m_depthStencil.view = m_logicalDevice.createImageView(depthStencilView);
}

void VulkanRendererBase::SetupRenderPass()
{
    Logger::Log("Setting up the render pass");

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
    subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;
    subpassDescription.pResolveAttachments = nullptr;

    std::array<vk::SubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
    dependencies[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;


    std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    vk::RenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassCreateInfo.pDependencies = dependencies.data();

    m_renderPass = m_logicalDevice.createRenderPass(renderPassCreateInfo);
}

void VulkanRendererBase::CreatePipelineCache()
{
    vk::PipelineCacheCreateInfo pipelineCacheCreateInfo;
    m_pipelineCache = m_logicalDevice.createPipelineCache(pipelineCacheCreateInfo);
}

void VulkanRendererBase::SetupFrameBuffer()
{
    Logger::Log("Setting up framebuffer");

    m_frameBuffers.resize(m_swapchain.GetImageViewCount());

    auto swapChainImageViews = m_swapchain.GetImageViews();

    for (size_t i = 0; i < m_swapchain.GetImageViewCount(); i++)
    {
        std::array<vk::ImageView, 2> attachments = {
                swapChainImageViews[i],
                m_depthStencil.view
        };

        vk::FramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = m_swapchain.GetExtent().width;
        framebufferInfo.height = m_swapchain.GetExtent().height;
        framebufferInfo.layers = 1;

        m_frameBuffers[i] = m_logicalDevice.createFramebuffer(framebufferInfo);
    }
}

void VulkanRendererBase::RenderFrame()
{
    auto tStart = std::chrono::high_resolution_clock::now();
    Render();
    auto tEnd = std::chrono::high_resolution_clock::now();
    auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
    //std::cout << "[FrameTime] " << 	(float)tDiff / 1000.0f << std::endl;

}

void VulkanRendererBase::Cleanup()
{
    Logger::Log("Cleaning up");
    m_swapchain.Cleanup();
}

void VulkanRendererBase::DeviceWaitIdle()
{
    m_logicalDevice.waitIdle();
}

void VulkanRendererBase::RebuildSwapchain()
{
    //TODO: Make it so this does not crash
    //Operations must be done before cleaning up
    m_logicalDevice.waitIdle();

    DestroySwapchain();
    SetupSwapchain();

    DestroyDepthStencil();
    SetupDepthStencil();

    DestroyFrameBuffers();
    SetupFrameBuffer();

    DestroyCommandBuffers();
    CreateCommandBuffers();
    BuildCommandBuffers();

    m_logicalDevice.waitIdle();
}

void VulkanRendererBase::PrepareFrame()
{
    vk::Result swapchainStatus = m_swapchain.AcquireNextImage(m_semaphores.presentComplete, &m_currentBuffer);

    if (swapchainStatus == vk::Result::eErrorOutOfDateKHR || swapchainStatus == vk::Result::eSuboptimalKHR)
    {
        //Rebuild swapchain
        Logger::Log("Swapchain is not running optimal anymore, please reload it");
    }
    else if(swapchainStatus != vk::Result::eSuccess)
    {
        std::cout << vk::to_string(swapchainStatus) << std::endl;
        throw std::runtime_error("Failed to acquire swap chain image!");
    }
}

void VulkanRendererBase::SubmitFrame()
{
    m_swapchain.QueuePresent(m_graphicsQueue, m_currentBuffer, m_semaphores.renderComplete);
    m_graphicsQueue.waitIdle();
}

vk::PipelineShaderStageCreateInfo VulkanRendererBase::LoadShader(const std::string &p_fileName, vk::ShaderStageFlagBits p_shaderStage)
{
    vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo;
    pipelineShaderStageCreateInfo.stage = p_shaderStage;
    pipelineShaderStageCreateInfo.module = VulkanHelpers::LoadShader(p_fileName.c_str(), m_logicalDevice);
    pipelineShaderStageCreateInfo.pName = "main"; //Entry point in shader
    m_shaderModules.push_back(pipelineShaderStageCreateInfo.module);
    return pipelineShaderStageCreateInfo;
}

VulkanRendererBase::VulkanRendererBase()
{
}

VulkanRendererBase::~VulkanRendererBase()
{
    Logger::Log("Cleaning up stuff");

    m_swapchain.Cleanup();

    if(m_descriptorPool) m_logicalDevice.destroyDescriptorPool(m_descriptorPool);

    m_logicalDevice.freeCommandBuffers(m_commandPool, m_drawCommandBuffers);

    m_logicalDevice.destroyRenderPass(m_renderPass);

    for (size_t i = 0; i < m_frameBuffers.size(); ++i)
    {
        m_logicalDevice.destroyFramebuffer(m_frameBuffers[i]);
    }

    for (size_t i = 0; i < m_shaderModules.size(); ++i)
    {
        m_logicalDevice.destroyShaderModule(m_shaderModules[i]);
    }

    //Destroying depthstencil
    m_logicalDevice.destroyImageView(m_depthStencil.view);
    m_logicalDevice.destroyImage(m_depthStencil.image);
    m_logicalDevice.freeMemory(m_depthStencil.memory);

    m_logicalDevice.destroyPipelineCache(m_pipelineCache);

    m_logicalDevice.destroyCommandPool(m_commandPool);

    m_logicalDevice.destroySemaphore(m_semaphores.renderComplete);
    m_logicalDevice.destroySemaphore(m_semaphores.presentComplete);

    delete m_wrappedDevice;

    m_instance.destroy();
}

void VulkanRendererBase::DestroyCommandBuffers()
{
    m_logicalDevice.freeCommandBuffers(m_commandPool, static_cast<uint32_t>(m_drawCommandBuffers.size()), m_drawCommandBuffers.data());
}

void VulkanRendererBase::DestroyDepthStencil()
{
    m_logicalDevice.destroyImageView(m_depthStencil.view);
    m_logicalDevice.destroyImage(m_depthStencil.image);
    m_logicalDevice.freeMemory(m_depthStencil.memory);
}

void VulkanRendererBase::DestroySwapchain()
{
    m_swapchain.Cleanup();
}

void VulkanRendererBase::DestroyFrameBuffers()
{
    for (size_t i = 0; i < m_frameBuffers.size(); ++i)
    {
        m_logicalDevice.destroyFramebuffer(m_frameBuffers[i]);
    }
}

WrappedVulkanWindow * VulkanRendererBase::GetWindow()
{
    return m_window;
}

void VulkanRendererBase::CreateSemaphores()
{
    // Create synchronization objects
    vk::SemaphoreCreateInfo semaphoreCreateInfo;

    m_semaphores.presentComplete = m_logicalDevice.createSemaphore(semaphoreCreateInfo);

    m_semaphores.renderComplete = m_logicalDevice.createSemaphore(semaphoreCreateInfo);

    m_submitInfo.pWaitDstStageMask    = &m_submitPipelineStages;
    m_submitInfo.waitSemaphoreCount   = 1;
    m_submitInfo.pWaitSemaphores      = &m_semaphores.presentComplete;
    m_submitInfo.signalSemaphoreCount = 1;
    m_submitInfo.pSignalSemaphores    = &m_semaphores.renderComplete;
}