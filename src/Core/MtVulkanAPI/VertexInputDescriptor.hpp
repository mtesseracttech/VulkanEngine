//
// Created by MTesseracT on 13-1-2018.
//

#ifndef VULKANENGINE_VERTEXINPUTDESCRIPTOR_HPP
#define VULKANENGINE_VERTEXINPUTDESCRIPTOR_HPP

#include <utility>
#include <vulkan/vulkan.hpp>
#include "VulkanModel.hpp"

class VertexInputDescriptor
{
    std::vector<vk::VertexInputBindingDescription>   m_vertexInputBindings;
    std::vector<vk::VertexInputAttributeDescription> m_vertexInputAttributes;
    VertexLayout                                     m_vertexLayout;

public:
    explicit VertexInputDescriptor(VertexLayout p_vertexLayout) : m_vertexLayout(std::move(p_vertexLayout))
    {
        GenerateBindingDescription();
        GenerateAttributeDescription();
    };

    vk::VertexInputBindingDescription *GetBindings()
    {
        return m_vertexInputBindings.data();
    }

    vk::VertexInputAttributeDescription *GetAttributes()
    {
        return m_vertexInputAttributes.data();
    }

    uint32_t GetBindingCount()
    {
        return static_cast<uint32_t>(m_vertexInputBindings.size());
    }

    uint32_t GetAttributeCount()
    {
        return static_cast<uint32_t >(m_vertexInputAttributes.size());
    }

private:
    void GenerateBindingDescription()
    {
        vk::VertexInputBindingDescription vertexInputBindingDescription;
        vertexInputBindingDescription.binding   = 0;
        vertexInputBindingDescription.stride    = m_vertexLayout.GetStride();
        vertexInputBindingDescription.inputRate = vk::VertexInputRate::eVertex;

        m_vertexInputBindings = {vertexInputBindingDescription};
    }

    void GenerateAttributeDescription()
    {
        m_vertexInputAttributes.resize(m_vertexLayout.m_vertexComponents.size());

        uint32_t offset = 0;

        for (uint32_t i = 0; i < m_vertexLayout.m_vertexComponents.size(); ++i)
        {
            const auto &comp = m_vertexLayout.m_vertexComponents[i];

            vk::VertexInputAttributeDescription description;
            description.binding  = 0;
            description.location = i;
            description.offset   = offset;

            switch (comp)
            {
                case eDummyFloat:
                    description.format = vk::Format::eR32Sfloat;
                    offset += 1 * sizeof(float);
                    break;
                case eUV:
                    description.format = vk::Format::eR32G32Sfloat;
                    offset += 2 * sizeof(float);
                    break;
                case eDummyVec4:
                    description.format = vk::Format::eR32G32B32A32Sfloat;
                    offset += 4 * sizeof(float);
                    break;
                default:
                    description.format = vk::Format::eR32G32B32Sfloat;
                    offset += 3 * sizeof(float);
            }

            m_vertexInputAttributes[i] = description;
        }
    }
};

#endif //VULKANENGINE_VERTEXINPUTDESCRIPTOR_HPP
