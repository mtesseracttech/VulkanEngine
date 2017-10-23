//
// Created by MTesseracT on 23-10-2017.
//

#ifndef VULKANENGINE_VULKANMODEL_HPP
#define VULKANENGINE_VULKANMODEL_HPP


#include <vulkan/vulkan.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include "WrappedVulkanBuffer.hpp"

enum VulkanVertexComponent
{
    ePosition,
    eNormal,
    eColor,
    eUV,
    eTangent,
    eBitangent,
    eDummyFloat,
    eDummyVec4
};


struct VertexLayout
{
    std::vector<VulkanVertexComponent> m_vertexComponents;

    explicit VertexLayout(std::vector<VulkanVertexComponent> components)
    {
        m_vertexComponents = std::move(components);
    }

    uint32_t GetStride()
    {
        uint32_t res = 0;
        for (auto& component : m_vertexComponents)
        {
            switch (component)
            {
                case eUV:
                    res += 2 * sizeof(float);
                    break;
                case eDummyFloat:
                    res += sizeof(float);
                    break;
                case eDummyVec4:
                    res += 4 * sizeof(float);
                    break;
                default:
                    res += 3 * sizeof(float);
            }
        }
        return res;
    }
};


struct VulkanModel
{
    vk::Device device = nullptr;
    WrappedVulkanBuffer vertexBuffer;
    WrappedVulkanBuffer indexBuffer;
    uint32_t indexCount;
    uint32_t vertexCount;

    struct ModelPart
    {
        uint32_t vertexBase;
        uint32_t vertexCount;
        uint32_t indexBase;
        uint32_t indexCount;
    };

    std::vector<ModelPart> parts;

    static const int defaultFlags = aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;


};


#endif //VULKANENGINE_VULKANMODEL_HPP
