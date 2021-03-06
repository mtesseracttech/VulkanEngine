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

    explicit operator vk::Device()   { return m_logicalDevice; };

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
                             const vk::QueueFlags &p_requestedQueueTypes = vk::QueueFlagBits::eGraphics |
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

    vk::Result CreateBuffer(vk::BufferUsageFlags usageFlags, vk::MemoryPropertyFlags memoryPropertyFlags, vk::DeviceSize size, vk::Buffer *buffer, vk::DeviceMemory *memory, void *data = nullptr)
    {
        // Create the buffer handle
        //vk::BufferCreateInfo bufferCreateInfo = vks::initializers::bufferCreateInfo(usageFlags, size);
        vk::BufferCreateInfo bufferCreateInfo;
        bufferCreateInfo.usage = usageFlags;
        bufferCreateInfo.size = size;
        bufferCreateInfo.sharingMode = vk::SharingMode ::eExclusive;
        *buffer = m_logicalDevice.createBuffer(bufferCreateInfo);

        // Create the memory backing up the buffer handle
        vk::MemoryRequirements memReqs = m_logicalDevice.getBufferMemoryRequirements(*buffer);

        vk::MemoryAllocateInfo memAlloc;
        memAlloc.allocationSize = memReqs.size;
        memAlloc.memoryTypeIndex = GetMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);

        *memory = m_logicalDevice.allocateMemory(memAlloc);
        //m_logicalDevice.allocateMemory(&memAlloc, nullptr, memory);

        // If a pointer to the buffer data has been passed, map the buffer and copy over the data
        if (data != nullptr)
        {
            void *mapped;
            mapped = m_logicalDevice.mapMemory(*memory, 0, size, static_cast<vk::MemoryMapFlagBits>(0));
            memcpy(mapped, data, size);
            // If host coherency hasn't been requested, do a manual flush to make writes visible
            if ((memoryPropertyFlags & vk::MemoryPropertyFlagBits ::eHostCoherent) == vk::MemoryPropertyFlagBits(0))
            {
                vk::MappedMemoryRange mappedRange;
                mappedRange.memory = *memory;
                mappedRange.offset = 0;
                mappedRange.size = size;
                m_logicalDevice.flushMappedMemoryRanges(1, &mappedRange);
                //vkFlushMappedMemoryRanges(logicalDevice, 1, &mappedRange);
            }
            m_logicalDevice.unmapMemory(*memory);
        }

        // Attach the memory to the buffer object
        m_logicalDevice.bindBufferMemory(*buffer, *memory, 0);
        //VK_CHECK_RESULT(vkBindBufferMemory(logicalDevice, *buffer, *memory, 0));

        return vk::Result::eSuccess;
    }


    vk::Result CreateBuffer(vk::BufferUsageFlags usageFlags, vk::MemoryPropertyFlags memoryPropertyFlags, WrappedVulkanBuffer *buffer, vk::DeviceSize size, void *data = nullptr)
    {
        buffer->m_device = m_logicalDevice;

        // Create the buffer handle
        vk::BufferCreateInfo bufferCreateInfo;
        bufferCreateInfo.usage = usageFlags;
        bufferCreateInfo.size = size;

        buffer->m_buffer = m_logicalDevice.createBuffer(bufferCreateInfo);

        // Create the memory backing up the buffer handle
        vk::MemoryRequirements memReqs = m_logicalDevice.getBufferMemoryRequirements(buffer->m_buffer);

        vk::MemoryAllocateInfo memAlloc;
        memAlloc.allocationSize = memReqs.size;
        memAlloc.memoryTypeIndex = GetMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);

        buffer->m_memory = m_logicalDevice.allocateMemory(memAlloc);
        buffer->m_alignment = memReqs.alignment;
        buffer->m_size = memAlloc.allocationSize;
        buffer->m_usageFlags = usageFlags;
        buffer->m_memoryPropertyFlags = memoryPropertyFlags;

        // If a pointer to the buffer data has been passed, map the buffer and copy over the data
        if (data != nullptr)
        {
            buffer->Map();
            //VK_CHECK_RESULT(buffer->map());
            memcpy(buffer->m_mapped, data, size);
            buffer->Unmap();
        }

        // Initialize a default descriptor that covers the whole buffer size
        buffer->SetupDescriptor();

        // Attach the memory to the buffer object
        buffer->Bind();
        return vk::Result ::eSuccess;
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
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo;
        commandBufferAllocateInfo.commandPool        = m_commandPool;
        commandBufferAllocateInfo.level              = p_level;
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
