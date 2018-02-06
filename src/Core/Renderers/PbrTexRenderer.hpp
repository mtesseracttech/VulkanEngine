//
// Created by MTesseracT on 6-2-2018.
//

#ifndef VULKANENGINE_PBRTEXRENDERER_HPP
#define VULKANENGINE_PBRTEXRENDERER_HPP

#include <Core/MtVulkanAPI/VulkanRendererBase.hpp>
#include <Core/MtVulkanAPI/VulkanModel.hpp>
#include <Material/SimplePbrMaterial.hpp>
#include <Core/Camera.hpp>

class PbrTexRenderer : public VulkanRendererBase
{
//Variables
public:
    vk::DescriptorSetLayout  m_descriptorSetLayout;
    vk::DescriptorSet        m_descriptorSet;
    vk::PipelineLayout       m_pipelineLayout;
    vk::Pipeline             m_pipeline;
    std::vector<std::string> m_materialNames;
    std::vector<std::string> m_objectNames;
    Camera                   m_camera;

    VertexLayout m_vertexLayout = VertexLayout({
                                                       VulkanVertexComponent::ePosition,
                                                       VulkanVertexComponent::eNormal,
                                                       VulkanVertexComponent::eUV}
    );

    struct Meshes
    {
        std::vector<VulkanModel> m_models;
        int32_t                  m_objectIndex      = 0;
    } m_objects;

    std::vector<SimplePbrMaterial> m_materials;
    int32_t                        m_materialIndex  = 9;

    struct UniformBuffers
    {
        WrappedVulkanBuffer m_object;
        WrappedVulkanBuffer m_params;
    } m_uniformBuffers;

    struct UboMatrices
    {
        glm::mat4 m_model;
        glm::mat4 m_view;
        glm::mat4 m_projection;
        glm::vec3 m_cameraPos;
    } m_uboMatrices;

    struct UboParameters
    {
        glm::vec4 m_lights[4];
    } m_uboParameters;

    void UpdateUniformBuffers();

protected:

private:
    const uint32_t VERTEX_BUFFER_BIND_ID = 0;

//Methods
public:
    PbrTexRenderer();

    ~PbrTexRenderer() override;

protected:
    void BuildCommandBuffers() override;

    void Render() override;

    void Prepare() override;

    void Cleanup() override;

private:
    void SetupMaterials();

    void SetupCamera();

    void SetupModels();

    void SetupUniformBuffers();

    void SetupDescriptorSetLayout();

    void SetupPipelineLayout();

    void SetupPipeline();

    void SetupDescriptorSets();
};


#endif //VULKANENGINE_PBRTEXRENDERER_HPP
