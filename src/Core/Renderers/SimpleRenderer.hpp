//
// Created by MTesseracT on 30-10-2017.
//

#ifndef VULKANENGINE_SIMPLERENDERER_HPP
#define VULKANENGINE_SIMPLERENDERER_HPP

#include <Core/MtVulkanAPI/VulkanRendererBase.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <gli/gli.hpp>

#include <Core/MtVulkanAPI/VulkanModel.hpp>
#include <Core/Camera.hpp>

#include <vector>
#include <Core/MtVulkanAPI/VulkanTexture.hpp>


class SimpleRenderer : public VulkanRendererBase
{
    VertexLayout m_vertexLayout = VertexLayout(
            {VulkanVertexComponent ::ePosition,
             VulkanVertexComponent ::eNormal,
             VulkanVertexComponent ::eUV,
             VulkanVertexComponent ::eColor}
    );

    const uint32_t VERTEX_BUFFER_BIND_ID = 0;

    struct VertexInfo{
        vk::PipelineVertexInputStateCreateInfo           inputState;
        std::vector<vk::VertexInputBindingDescription>   bindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
    } m_vertexInfo;


    struct {
        glm::mat4 projection;
        glm::mat4 modelView;
        glm::vec4 lightPos = glm::vec4(0.0f, 2.0f, 1.0f, 0.0f);
    } m_ubo;

    //Info that individual objects should store
    struct {
        VulkanModel centerModel;
        VulkanModel skybox;
    } m_models;

    struct {
        WrappedVulkanBuffer         centerObject;
        WrappedVulkanBuffer         skybox;
    } m_uniformBuffers;

    struct {
        vk::Pipeline            centerObject;
        vk::Pipeline            skybox;
    } m_pipelines;

    struct{
        vk::DescriptorSet       centerObject;
        vk::DescriptorSet       skybox;
    } m_descriptorSets;


    //Layouts
    vk::PipelineLayout          m_pipelineLayout;
    vk::DescriptorSetLayout     m_descriptorSetLayout;

    glm::vec3                   m_cameraPosition = glm::vec3();
    glm::vec3                   m_cameraRotation = glm::vec3(-25.0f, 15.0f, 0.0f);
    float                       m_cameraZoom     = -10.5f;

    VulkanCubemap               m_skyboxTex;

    Camera*                     m_camera; //real camera

public:
    SimpleRenderer();

    ~SimpleRenderer() override;

    void Prepare() override;

    void GetEnabledFeatures() override;

    void Render() override;

    void BuildCommandBuffers() override;

    void LoadSkyboxAssets();

    void LoadModels();

    void PrepareUniformBuffers();

    void UpdateUniformBuffers();

    void SetupDescriptorSetLayout();

    void PreparePipeline();

    void SetupDescriptorPool();

    void SetupDescriptorSets();

    void CreateCubemap(const std::string &p_filename, vk::Format p_format);

    void SetupVertexDescriptions();
};


#endif //VULKANENGINE_SIMPLERENDERER_HPP
