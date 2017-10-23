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
#include <glm/glm.hpp>

#include <sstream>
#include <Utility/Logger.hpp>
#include "WrappedVulkanBuffer.hpp"
#include "WrappedVulkanDevice.hpp"

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
        for (auto &component : m_vertexComponents)
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
    WrappedVulkanBuffer m_vertexBuffer;
    WrappedVulkanBuffer m_indexBuffer;
    uint32_t m_indexCount;
    uint32_t m_vertexCount;

    struct ModelPart
    {
        uint32_t vertexBase;
        uint32_t vertexCount;
        uint32_t indexBase;
        uint32_t indexCount;
    };

    std::vector<ModelPart> m_parts;

    static const int defaultFlags = aiProcess_FlipWindingOrder
                                    | aiProcess_Triangulate
                                    | aiProcess_PreTransformVertices
                                    | aiProcess_CalcTangentSpace
                                    | aiProcess_GenSmoothNormals;

    struct Dimensions
    {
        glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max());
        glm::vec3 size;
    } m_dimensions;

    void Destroy()
    {
        m_vertexBuffer.Destroy();
        if (m_indexBuffer.m_buffer)
        {
            m_indexBuffer.Destroy();
        }
    }

    bool LoadFromFile(const std::string &p_filename, VertexLayout p_layout, WrappedVulkanDevice *p_device,
                      vk::Queue p_copyQueue, const int p_assimpFlags = defaultFlags)
    {
        Assimp::Importer importer;
        const aiScene *scene;

        scene = importer.ReadFile(p_filename.c_str(), p_assimpFlags);

        if (scene)
        {
            m_parts.clear();
            m_parts.resize(scene->mNumMeshes);

            glm::vec3 scale(1.0f);
            glm::vec2 uvscale(1.0f);
            glm::vec3 center(0.0f);

            std::vector<float> vertexBuffer;
            std::vector<uint32_t> indexBuffer;

            m_vertexCount = 0;
            m_indexCount = 0;

            for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
            {
                const aiMesh *aiMesh = scene->mMeshes[i];

                m_parts[i] = {};
                m_parts[i].vertexBase = m_vertexCount;
                m_parts[i].indexBase = m_indexCount;

                m_vertexCount += scene->mMeshes[i]->mNumVertices;

                aiColor3D color(0.f, 0.f, 0.f);
                scene->mMaterials[aiMesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);

                const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

                for (unsigned int j = 0; j < aiMesh->mNumVertices; j++)
                {
                    const aiVector3D *pos = &(aiMesh->mVertices[j]);
                    const aiVector3D *normal = &(aiMesh->mNormals[j]);
                    const aiVector3D *texCoord = (aiMesh->HasTextureCoords(0)) ? &(aiMesh->mTextureCoords[0][j])
                                                                               : &Zero3D;
                    const aiVector3D *tangent = (aiMesh->HasTangentsAndBitangents()) ? &(aiMesh->mTangents[j])
                                                                                     : &Zero3D;
                    const aiVector3D *biTangent = (aiMesh->HasTangentsAndBitangents()) ? &(aiMesh->mBitangents[j])
                                                                                       : &Zero3D;

                    for (auto &component : p_layout.m_vertexComponents)
                    {
                        switch (component)
                        {
                            case ePosition:
                                vertexBuffer.push_back(pos->x * scale.x + center.x);
                                vertexBuffer.push_back(-pos->y * scale.y + center.y);
                                vertexBuffer.push_back(pos->z * scale.z + center.z);
                                break;
                            case eNormal:
                                vertexBuffer.push_back(normal->x);
                                vertexBuffer.push_back(-normal->y);
                                vertexBuffer.push_back(normal->z);
                                break;
                            case eUV:
                                vertexBuffer.push_back(texCoord->x * uvscale.s);
                                vertexBuffer.push_back(texCoord->y * uvscale.t);
                                break;
                            case eColor:
                                vertexBuffer.push_back(color.r);
                                vertexBuffer.push_back(color.g);
                                vertexBuffer.push_back(color.b);
                                break;
                            case eTangent:
                                vertexBuffer.push_back(tangent->x);
                                vertexBuffer.push_back(tangent->y);
                                vertexBuffer.push_back(tangent->z);
                                break;
                            case eBitangent:
                                vertexBuffer.push_back(biTangent->x);
                                vertexBuffer.push_back(biTangent->y);
                                vertexBuffer.push_back(biTangent->z);
                                break;
                            case eDummyFloat:
                                vertexBuffer.push_back(0.0f);
                                break;
                            case eDummyVec4:
                                vertexBuffer.push_back(0.0f);
                                vertexBuffer.push_back(0.0f);
                                vertexBuffer.push_back(0.0f);
                                vertexBuffer.push_back(0.0f);
                                break;
                        };
                    }

                    m_dimensions.max.x = static_cast<float>(fmax(pos->x, m_dimensions.max.x));
                    m_dimensions.max.y = static_cast<float>(fmax(pos->y, m_dimensions.max.y));
                    m_dimensions.max.z = static_cast<float>(fmax(pos->z, m_dimensions.max.z));

                    m_dimensions.min.x = static_cast<float>(fmin(pos->x, m_dimensions.min.x));
                    m_dimensions.min.y = static_cast<float>(fmin(pos->y, m_dimensions.min.y));
                    m_dimensions.min.z = static_cast<float>(fmin(pos->z, m_dimensions.min.z));
                }

                m_dimensions.size = m_dimensions.max - m_dimensions.min;

                m_parts[i].vertexCount = aiMesh->mNumVertices;

                uint32_t indexBase = static_cast<uint32_t>(indexBuffer.size());

                for (unsigned int j = 0; j < aiMesh->mNumFaces; j++)
                {
                    const aiFace& Face = aiMesh->mFaces[j];
                    if (Face.mNumIndices != 3)
                        continue;
                    indexBuffer.push_back(indexBase + Face.mIndices[0]);
                    indexBuffer.push_back(indexBase + Face.mIndices[1]);
                    indexBuffer.push_back(indexBase + Face.mIndices[2]);
                    m_parts[i].indexCount += 3;
                    m_indexCount += 3;
                }
            }

            uint32_t vertexBufferSize = static_cast<uint32_t>(vertexBuffer.size()) * sizeof(float);
            uint32_t indexBufferSize = static_cast<uint32_t>(indexBuffer.size()) * sizeof(uint32_t);

            //Create staging buffers

            WrappedVulkanBuffer vertexStaging = p_device->CreateBuffer(
                    vk::BufferUsageFlagBits::eTransferSrc,
                    vk::MemoryPropertyFlagBits::eHostVisible,
                    vertexBufferSize,
                    vertexBuffer.data());

            WrappedVulkanBuffer indexStaging = p_device->CreateBuffer(
                    vk::BufferUsageFlagBits::eTransferSrc,
                    vk::MemoryPropertyFlagBits::eHostVisible,
                    indexBufferSize,
                    indexBuffer.data());

            //Local target buffers

            m_vertexBuffer = p_device->CreateBuffer(
                    vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    vertexBufferSize);

            m_indexBuffer = p_device->CreateBuffer(
                    vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                    vk::MemoryPropertyFlagBits::eDeviceLocal,
                    indexBufferSize);

            //Copy from staging buffers

            vk::CommandBuffer copyCommand = p_device->CreateCommandBuffer(vk::CommandBufferLevel::ePrimary, true);

            vk::BufferCopy copyRegion;

            copyRegion.size = m_vertexBuffer.m_size;

            copyCommand.copyBuffer(vertexStaging.m_buffer, m_vertexBuffer.m_buffer, 1, &copyRegion);

            copyRegion.size = m_vertexBuffer.m_size;

            p_device->FlushCommandBuffer(copyCommand, p_copyQueue);

            vk::Device device = p_device->m_logicalDevice;

            vertexStaging.Destroy();
            indexStaging.Destroy();

            return true;
        }
        else
        {
            std::stringstream output;
            output << "Error parsing '" << p_filename.c_str() << "': '" << importer.GetErrorString() << "'";
            Logger::Log(output.str(), LogType::LogError);

            return false;
        }
    }
};


#endif //VULKANENGINE_VULKANMODEL_HPP
