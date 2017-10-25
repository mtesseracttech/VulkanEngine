//
// Created by MTesseracT on 19-10-2017.
//

#ifndef VULKANENGINE_WRAPPEDVULKANDEVICE_HPP
#define VULKANENGINE_WRAPPEDVULKANDEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include "WrappedVulkanBuffer.hpp"


struct WrappedVulkanDevice
{
    vk::PhysicalDevice                      m_physicalDevice;
    vk::Device                              m_logicalDevice     = nullptr;
    vk::PhysicalDeviceProperties            m_deviceProperties;
    vk::PhysicalDeviceFeatures              m_deviceFeatures;
    vk::PhysicalDeviceFeatures              m_enabledFeatures;
    vk::PhysicalDeviceMemoryProperties      m_memoryProperties;
    std::vector<vk::QueueFamilyProperties>  m_queueFamilyProperties;
    std::vector<std::string>                m_supportedExtensions;
    vk::CommandPool                         m_commandPool       = nullptr;

    struct
    {
        uint32_t graphics;
        uint32_t compute;
        uint32_t transfer;
    } m_queueFamilyIndices;

    operator vk::Device()   { return m_logicalDevice; };

    WrappedVulkanDevice(vk::PhysicalDevice p_physicalDevice) : m_deviceProperties(vk::PhysicalDeviceProperties())
    {
        assert(p_physicalDevice);

        m_physicalDevice = p_physicalDevice;
        m_deviceProperties = m_physicalDevice.getProperties();
        m_deviceFeatures = m_physicalDevice.getFeatures();
        m_memoryProperties = m_physicalDevice.getMemoryProperties();
        m_queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

        assert(!m_queueFamilyProperties.empty());

        std::vector<vk::ExtensionProperties> extensions = m_physicalDevice.enumerateDeviceExtensionProperties();
        for(const auto& extension : extensions)
        {
            m_supportedExtensions.push_back(extension.extensionName);
        }
    }

    ~WrappedVulkanDevice()
    {
        if (m_commandPool)
        {
            m_logicalDevice.destroyCommandPool(m_commandPool);
        }
        if (m_logicalDevice)
        {
            m_logicalDevice.destroy();
        }
    }

    uint32_t GetMemoryType(uint32_t p_typeBits, const vk::MemoryPropertyFlags p_properties, vk::Bool32 * p_memTypeFound = nullptr)
    {
        //Cycling through the available memory types and checking if the property flags match those requested
        for (uint32_t i = 0; i < m_memoryProperties.memoryTypeCount; ++i)
        {
            if((p_typeBits & 1) == 1)
            {
                if ((m_memoryProperties.memoryTypes[i].propertyFlags & p_properties) == p_properties)
                {
                    if (p_memTypeFound)
                    {
                        *p_memTypeFound = static_cast<vk::Bool32>(true);
                    }
                    return i;
                }
            }
            p_typeBits >>= 1;
        }

        //If the optional p_memTypeFound is passed too and accounted for, not finding memory will not result in an error.
        if (p_memTypeFound)
        {
            *p_memTypeFound = static_cast<vk::Bool32>(false);
            return 0;
        }
        else
        {
            throw std::runtime_error("Could not find a matching memory type");
        }
    }

    uint32_t GetQueueFamilyIndex(vk::QueueFlagBits p_flagBits)
    {
        if (static_cast<VkQueueFlagBits>(p_flagBits) & static_cast<VkQueueFlagBits>(vk::QueueFlagBits::eCompute))
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(m_queueFamilyProperties.size()); ++i)
            {
                //Might give problems
                if ((m_queueFamilyProperties[i].queueFlags & p_flagBits) && (static_cast<uint32_t>(m_queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) == 0))
                {
                    return i;
                }
            }
        }

        if (static_cast<VkQueueFlagBits>(p_flagBits) & static_cast<VkQueueFlagBits>(vk::QueueFlagBits::eTransfer))
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(m_queueFamilyProperties.size()); ++i)
            {
                if ((m_queueFamilyProperties[i].queueFlags & p_flagBits) && (static_cast<uint32_t>(m_queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) == 0) && (static_cast<uint32_t>(m_queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute) == 0))
                {
                    return i;
                }
            }
        }

        for (uint32_t i = 0; i < static_cast<uint32_t>(m_queueFamilyProperties.size()); ++i)
        {
            if (m_queueFamilyProperties[i].queueFlags & p_flagBits)
            {
                return i;
            }
        }

        throw std::runtime_error("Could not find a matching queue family index");
    }

    void CreateLogicalDevice(vk::PhysicalDeviceFeatures p_enabledFeatures,
                             std::vector<const char *> p_enabledExtensions, bool p_useSwapChain = true,
                             vk::QueueFlags p_requestedQueueTypes = vk::QueueFlagBits::eGraphics |
                                                                    vk::QueueFlagBits::eCompute)
    {
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};

        const float defaultQueuePriority(0.0f);

        if (p_requestedQueueTypes & vk::QueueFlagBits::eGraphics)
        {

            m_queueFamilyIndices.graphics = GetQueueFamilyIndex(vk::QueueFlagBits::eGraphics);
            vk::DeviceQueueCreateInfo queueInfo{};
            queueInfo.queueFamilyIndex = m_queueFamilyIndices.graphics;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &defaultQueuePriority;
            queueCreateInfos.push_back(queueInfo);
        }
        else
        {
            m_queueFamilyIndices.graphics = VK_NULL_HANDLE;
        }

        if (p_requestedQueueTypes & vk::QueueFlagBits::eCompute)
        {
            m_queueFamilyIndices.compute = GetQueueFamilyIndex(vk::QueueFlagBits::eCompute);
            if (m_queueFamilyIndices.compute != m_queueFamilyIndices.graphics)
            {
                vk::DeviceQueueCreateInfo queueInfo{};
                queueInfo.queueFamilyIndex = m_queueFamilyIndices.compute;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);
            }
        }
        else
        {
            m_queueFamilyIndices.compute = m_queueFamilyIndices.graphics;
        }

        if (p_requestedQueueTypes & vk::QueueFlagBits::eTransfer)
        {
            m_queueFamilyIndices.transfer = GetQueueFamilyIndex(vk::QueueFlagBits::eTransfer);
            if ((m_queueFamilyIndices.transfer != m_queueFamilyIndices.graphics) && (m_queueFamilyIndices.transfer != m_queueFamilyIndices.compute))
            {
                vk::DeviceQueueCreateInfo queueInfo{};
                queueInfo.queueFamilyIndex = m_queueFamilyIndices.transfer;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);
            }
        }
        else
        {
            m_queueFamilyIndices.transfer = m_queueFamilyIndices.graphics;
        }


        std::vector<const char*> deviceExtensions(p_enabledExtensions);
        if (p_useSwapChain)
        {
            deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        vk::DeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures = &p_enabledFeatures;

        if (deviceExtensions.size() > 0)
        {
            deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
            deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        }

        m_logicalDevice = m_physicalDevice.createDevice(deviceCreateInfo);

        m_commandPool = CreateCommandPool(m_queueFamilyIndices.graphics);

        m_enabledFeatures = p_enabledFeatures;
    }

    void CreateBuffer(vk::BufferUsageFlags p_usageFlags, vk::MemoryPropertyFlags p_memoryPropertyFlags,
                      vk::DeviceSize p_size, vk::Buffer *p_buffer, vk::DeviceMemory *p_memory, void *p_data = nullptr)
    {
        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.usage = p_usageFlags;
        bufferCreateInfo.size = p_size;
        bufferCreateInfo.sharingMode = vk::SharingMode::eExclusive;

        *p_buffer = m_logicalDevice.createBuffer(bufferCreateInfo, nullptr);

        vk::MemoryRequirements memoryRequirements;
        m_logicalDevice.getBufferMemoryRequirements(*p_buffer, &memoryRequirements);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = GetMemoryType(memoryRequirements.memoryTypeBits, p_memoryPropertyFlags);

        *p_memory = m_logicalDevice.allocateMemory(memoryAllocateInfo, nullptr);

        if(p_data != nullptr)
        {
            void* mapped;
            mapped = m_logicalDevice.mapMemory(*p_memory, 0, p_size);
            memcpy(mapped, p_data, p_size);

            if ((p_memoryPropertyFlags & vk::MemoryPropertyFlagBits::eHostCoherent) == static_cast<vk::MemoryPropertyFlagBits>(0))
            {
                vk::MappedMemoryRange mappedRange;
                mappedRange.memory = *p_memory;
                mappedRange.offset = 0;
                mappedRange.size = p_size;
                m_logicalDevice.flushMappedMemoryRanges(1, &mappedRange);
            }
            m_logicalDevice.unmapMemory(*p_memory);
        }

        m_logicalDevice.bindBufferMemory(*p_buffer, *p_memory, 0);
    }


    void CreateBuffer(vk::BufferUsageFlags p_usageFlags, vk::MemoryPropertyFlags p_memoryPropertyFlags, WrappedVulkanBuffer* p_buffer, vk::DeviceSize p_size, void *p_data = nullptr)
    {
        p_buffer->m_device = m_logicalDevice;

        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.usage = p_usageFlags;
        bufferCreateInfo.size = p_size;

        p_buffer->m_buffer = m_logicalDevice.createBuffer(bufferCreateInfo, nullptr);

        vk::MemoryRequirements memoryRequirements = m_logicalDevice.getBufferMemoryRequirements(p_buffer->m_buffer);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = GetMemoryType(memoryRequirements.memoryTypeBits, p_memoryPropertyFlags);

        p_buffer->m_memory = m_logicalDevice.allocateMemory(memoryAllocateInfo, nullptr);

        p_buffer->m_alignment = memoryRequirements.alignment;
        p_buffer->m_size = memoryAllocateInfo.allocationSize;
        p_buffer->m_usageFlags = p_usageFlags;
        p_buffer->m_memoryPropertyFlags = p_memoryPropertyFlags;

        if(p_data != nullptr)
        {
            p_buffer->Map();
            memcpy(p_buffer->m_mapped, p_data, p_size);
            p_buffer->Unmap();
        }

        p_buffer->SetupDescriptor();

        p_buffer->Bind();
    }

    WrappedVulkanBuffer CreateBuffer(vk::BufferUsageFlags p_usageFlags, vk::MemoryPropertyFlags p_memoryPropertyFlags, vk::DeviceSize p_size, void *p_data = nullptr)
    {
        WrappedVulkanBuffer newBuffer;

        newBuffer.m_device = m_logicalDevice;

        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.usage = p_usageFlags;
        bufferCreateInfo.size = p_size;

        newBuffer.m_buffer = m_logicalDevice.createBuffer(bufferCreateInfo, nullptr);

        vk::MemoryRequirements memoryRequirements = m_logicalDevice.getBufferMemoryRequirements(newBuffer.m_buffer);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = GetMemoryType(memoryRequirements.memoryTypeBits, p_memoryPropertyFlags);

        newBuffer.m_memory = m_logicalDevice.allocateMemory(memoryAllocateInfo, nullptr);

        newBuffer.m_alignment = memoryRequirements.alignment;
        newBuffer.m_size = memoryAllocateInfo.allocationSize;
        newBuffer.m_usageFlags = p_usageFlags;
        newBuffer.m_memoryPropertyFlags = p_memoryPropertyFlags;

        if(p_data != nullptr)
        {
            newBuffer.Map();
            memcpy(newBuffer.m_mapped, p_data, p_size);
            newBuffer.Unmap();
        }

        newBuffer.SetupDescriptor();

        newBuffer.Bind();

        return newBuffer;
    }

    void CopyBuffer(WrappedVulkanBuffer* p_source, WrappedVulkanBuffer* p_destination, vk::Queue p_queue, vk::BufferCopy *p_copyRegion = nullptr)
    {
        assert(p_destination->m_size <= p_source->m_size);
        assert(p_destination->m_buffer && p_source->m_buffer);
        vk::CommandBuffer copyCmd = CreateCommandBuffer(vk::CommandBufferLevel::ePrimary, true);
        vk::BufferCopy bufferCopy{};
        if (p_copyRegion == nullptr)
        {
            bufferCopy.size = p_source->m_size;
        }
        else
        {
            bufferCopy = *p_copyRegion;
        }

        copyCmd.copyBuffer(p_source->m_buffer, p_destination->m_buffer, 1, &bufferCopy);
        FlushCommandBuffer(copyCmd, p_queue);
    }

    void FlushCommandBuffer(vk::CommandBuffer p_commandBuffer, vk::Queue p_queue, bool p_free = true)
    {
        if(!p_commandBuffer) return;

        p_commandBuffer.end();

        vk::SubmitInfo submitInfo;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &p_commandBuffer;

        vk::FenceCreateInfo fenceCreateInfo;
        fenceCreateInfo.flags = static_cast<vk::FenceCreateFlagBits>(0);

        vk::Fence fence = m_logicalDevice.createFence(fenceCreateInfo, nullptr);

        p_queue.submit(1, &submitInfo, fence);

        m_logicalDevice.waitForFences(1, &fence, VK_TRUE, 100000000000);

        m_logicalDevice.destroyFence(fence, nullptr);

        if(p_free)
        {
            m_logicalDevice.freeCommandBuffers(m_commandPool, 1, &p_commandBuffer);
        }
    }

    vk::CommandBuffer CreateCommandBuffer(vk::CommandBufferLevel p_level, bool p_begin = false)
    {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo;// = vks::initializers::commandBufferAllocateInfo(commandPool, level, 1);
        commandBufferAllocateInfo.commandPool = m_commandPool;
        commandBufferAllocateInfo.level = p_level;
        commandBufferAllocateInfo.commandBufferCount = 1;

        vk::CommandBuffer commandBuffer = m_logicalDevice.allocateCommandBuffers(commandBufferAllocateInfo)[0];

        if (p_begin)
        {
            vk::CommandBufferBeginInfo commandBufferInfo;
            commandBuffer.begin(commandBufferInfo);
        }

        return commandBuffer;
    }

    vk::CommandPool CreateCommandPool(uint32_t p_queueFamilyIndex, vk::CommandPoolCreateFlags p_createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
    {
        vk::CommandPoolCreateInfo poolCreateInfo = {};
        poolCreateInfo.queueFamilyIndex = p_queueFamilyIndex;
        poolCreateInfo.flags = p_createFlags;
        return m_logicalDevice.createCommandPool(poolCreateInfo);
    }

    bool ExtensionSupported(std::string extension)
    {
        return (std::find(m_supportedExtensions.begin(), m_supportedExtensions.end(), extension) != m_supportedExtensions.end());
    }
};


#endif //VULKANENGINE_WRAPPEDVULKANDEVICE_HPP
