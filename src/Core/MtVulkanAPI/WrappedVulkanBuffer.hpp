//
// Created by MTesseracT on 20-10-2017.
//

#ifndef VULKANENGINE_WRAPPEDVULKANBUFFER_HPP
#define VULKANENGINE_WRAPPEDVULKANBUFFER_HPP

#include <vulkan/vulkan.hpp>

struct WrappedVulkanBuffer
{
    vk::Device                  m_device                = nullptr;
    vk::Buffer                  m_buffer                = nullptr;
    vk::DeviceMemory            m_memory                = nullptr;
    vk::DescriptorBufferInfo    m_descriptor            ; //Is struct
    vk::DeviceSize              m_size                  = 0;
    vk::DeviceSize              m_alignment             = 0;
    void*                       m_mapped                = nullptr;
    vk::BufferUsageFlags        m_usageFlags;
    vk::MemoryPropertyFlags     m_memoryPropertyFlags;

    void Map(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0)
    {
        m_mapped = m_device.mapMemory(m_memory, offset, size, static_cast<vk::MemoryMapFlagBits>(0));
    }

    void Unmap()
    {
        if (m_mapped)
        {
            m_device.unmapMemory(m_memory);
            m_mapped = nullptr;
        }
    }

    void Bind(vk::DeviceSize p_offset = 0)
    {
        return m_device.bindBufferMemory(m_buffer, m_memory, p_offset);
    }

    void SetupDescriptor(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0)
    {
        m_descriptor.offset = p_offset;
        m_descriptor.buffer = m_buffer;
        m_descriptor.range = p_size;
    }

    void CopyTo(void* data, VkDeviceSize size)
    {
        assert(m_mapped);
        memcpy(m_mapped, data, size);
    }

    vk::Result Flush(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0)
    {
        vk::MappedMemoryRange mappedRange;
        mappedRange.memory = m_memory;
        mappedRange.offset = p_offset;
        mappedRange.size = p_size;
        return m_device.flushMappedMemoryRanges(1, &mappedRange);
    }

    vk::Result Invalidate(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0)
    {
        vk::MappedMemoryRange mappedRange;
        mappedRange.memory = m_memory;
        mappedRange.offset = p_offset;
        mappedRange.size = p_size;
        return m_device.invalidateMappedMemoryRanges(1, &mappedRange);
    }

    void Destroy()
    {
        if (m_buffer)
        {
            m_device.destroyBuffer(m_buffer, nullptr);
        }
        if (m_memory)
        {
            m_device.freeMemory(m_memory);
        }
    }
};


#endif //VULKANENGINE_WRAPPEDVULKANBUFFER_HPP
