//
// Created by MTesseracT on 19-10-2017.
//

#ifndef VULKANENGINE_WRAPPEDVULKANDEVICE_HPP
#define VULKANENGINE_WRAPPEDVULKANDEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include "Core/MtVulkanAPI/WrappedVulkanBuffer.hpp"


struct VulkanDevice
{
    vk::PhysicalDevice                 m_physicalDevice = nullptr;
    vk::Device                         m_logicalDevice  = nullptr;
    vk::PhysicalDeviceProperties       m_deviceProperties;
    vk::PhysicalDeviceFeatures         m_deviceFeatures;
    vk::PhysicalDeviceFeatures         m_enabledFeatures;
    vk::PhysicalDeviceMemoryProperties m_memoryProperties;
    vk::CommandPool                    m_commandPool    = nullptr;

    std::vector<vk::QueueFamilyProperties> m_queueFamilyProperties;
    std::vector<std::string>               m_supportedExtensions;

    struct
    {
        uint32_t graphics;
        uint32_t compute;
        uint32_t transfer;
    } m_queueFamilyIndices;

    explicit operator vk::Device();;

    VulkanDevice();

    ~VulkanDevice();

    void Create(vk::PhysicalDevice p_physicalDevice);

    void Destroy();

    uint32_t
    GetMemoryType(uint32_t p_typeBits, vk::MemoryPropertyFlags p_properties, vk::Bool32 *p_memTypeFound = nullptr);

    uint32_t GetQueueFamilyIndex(vk::QueueFlagBits p_flagBits);

    void CreateLogicalDevice(vk::PhysicalDeviceFeatures p_enabledFeatures,
                             std::vector<const char *> p_enabledExtensions, bool p_useSwapChain = true,
                             const vk::QueueFlags &p_requestedQueueTypes = vk::QueueFlagBits::eGraphics |
                                                                           vk::QueueFlagBits::eCompute);

    void CreateLogicalDevice(vk::PhysicalDeviceFeatures p_enabledFeatures,
                             std::vector<const char*> p_enabledExtensions,
                             const vk::QueueFlags& p_requestedQueueTypes = vk::QueueFlagBits::eGraphics |
                                                                           vk::QueueFlagBits::eCompute);

    vk::Result CreateBuffer(vk::BufferUsageFlags usageFlags,
                            vk::MemoryPropertyFlags memoryPropertyFlags,
                            vk::DeviceSize size,
                            vk::Buffer *buffer,
                            vk::DeviceMemory *memory,
                            void *data = nullptr);

    vk::Result CreateBuffer(vk::BufferUsageFlags usageFlags,
                            vk::MemoryPropertyFlags memoryPropertyFlags,
                            WrappedVulkanBuffer *buffer,
                            vk::DeviceSize size,
                            void *data = nullptr);

    void CopyBuffer(WrappedVulkanBuffer *p_source,
                    WrappedVulkanBuffer *p_destination,
                    vk::Queue p_queue,
                    vk::BufferCopy *p_copyRegion = nullptr);

    void FlushCommandBuffer(vk::CommandBuffer p_commandBuffer, vk::Queue p_queue, bool p_free = true);

    vk::CommandBuffer CreateCommandBuffer(vk::CommandBufferLevel p_level, bool p_begin = false);

    vk::CommandPool CreateCommandPool(uint32_t p_queueFamilyIndex,
                                      vk::CommandPoolCreateFlags p_createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

    bool ExtensionSupported(std::string extension);
};


#endif //VULKANENGINE_WRAPPEDVULKANDEVICE_HPP
