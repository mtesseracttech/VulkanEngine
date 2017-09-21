//
// Created by MTesseracT on 19-9-2017.
//

#include "VulkanEngine.hpp"

void VulkanEngine::Run()
{
    InitializeWindow();
    InitializeVulkan();
    MainLoop();
    Cleanup();
}

void VulkanEngine::InitializeWindow()
{
    m_window = new GlfwWindow("Vulkan GLFW Window", glm::ivec2(1200,900));
}

void VulkanEngine::InitializeVulkan()
{
    m_instance = new VulkanInstance(m_enableValidationLayers, &m_validationLayers);
    m_debug = new VulkanDebug(m_enableValidationLayers, &m_validationLayers, m_instance);
    //SetupDebugCallback();
    //CreateSurface();
    //PickPhysicalDevice();
    //CreateLogicalDevice();
    //CreateSwapChain();
    //CreateImageViews();
    //CreateRenderPass();
    //CreateGraphicsPipeline();
    //CreateFramebuffers();
    //CreateCommandPool();
    //CreateVertexBuffer();
    //CreateCommandBuffers();
    //CreateSemaphores();
}

void VulkanEngine::MainLoop()
{
    while (!glfwWindowShouldClose(m_window->GetWindow()))
    {
        glfwPollEvents();
        //DrawFrame();
    }

    //vk::Device::waitIdle();
}

void VulkanEngine::Cleanup()
{
//    CleanupSwapChain();
//    //Destroying Vertex Buffer and freeing up its memory
//    vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
//    vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
//    //Destroying semaphores
//    vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
//    vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
//    //Destroys the command pool
//    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
//    //Destroys the logical device
//    vkDestroyDevice(m_device, nullptr);
//    //Destroys the debug callback
//    DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
//    //Destroys the surface
//    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    //delete m_debug;
    delete m_instance;
    delete m_window;
}