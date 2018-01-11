//
// Created by mtesseract on 20-12-17.
//

#include "PbrRenderer.hpp"

PbrRenderer::PbrRenderer()
{
    Initialize();
    Prepare();
}

PbrRenderer::~PbrRenderer()
{
    Cleanup();
}

void PbrRenderer::Prepare()
{
    //Base Renderer Prepare Code First
    VulkanRendererBase::Prepare();

    //Renderer Specific Preparation Second
    SetupCamera();
    //Creating a list of the materials
    SetupMaterials();
    //Creating a list of the objects to be loaded
    SetupModels();
    //Preparing the uniform buffers
    SetupUniformBuffers();


}

void PbrRenderer::Cleanup()
{
    //Renderer Specific Cleanup Code First
    m_logicalDevice.destroyPipeline(m_pipeline);
    m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
    m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);

    for (auto& model : m_objects.m_models)
    {
        model.Destroy();
    }

    if(m_uniformBuffers.m_object.m_device)
    {
        m_uniformBuffers.m_object.Destroy();
    }

    if(m_uniformBuffers.m_params.m_device)
    {
        m_uniformBuffers.m_params.Destroy();
    }

    //Base Renderer Cleanup Second
    VulkanRendererBase::Cleanup();
}

void PbrRenderer::SetupCamera()
{
    //m_camera.SetPerspective(90.0f, m_window->GetAspectRatio(), 0.001f, 1000f);
    m_camera.SetCameraType(CameraType::FirstPerson);
    std::cout << m_window->GetAspectRatio() << std::endl;
    m_camera.SetPerspective(60f, m_window->GetAspectRatio(), 0.0001f, 1000f);
    m_camera.SetPosition(glm::vec3( 10.0f, 13.0f, 1.8f));
    m_camera.SetRotation(glm::vec3(-62.5f, 90.0f, 0.0f));
}

void PbrRenderer::SetupMaterials()
{
    //Metals
    m_materials.push_back(SimplePbrMaterial(glm::vec3(1.0f,      0.765557f, 0.336057f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.955008f, 0.637427f, 0.538163f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.549585f, 0.556114f, 0.554256f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.659777f, 0.608679f, 0.525649f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.541931f, 0.496791f, 0.449419f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.662124f, 0.654864f, 0.633732f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.672411f, 0.637331f, 0.585456f), 0.1f, 1.0f));
    // Testing materials
    m_materials.push_back(SimplePbrMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(1.0f, 0.0f, 0.0f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f));
    m_materials.push_back(SimplePbrMaterial(glm::vec3(0.0f, 0.0f, 1.0f), 0.1f, 1.0f));
}

void PbrRenderer::SetupModels()
{
    std::vector<std::string> filenames = { "teapot.dae" };
    for (const auto &file : filenames)
    {
        VulkanModel model;
        model.LoadFromFile(Constants::MODEL_PATH + file, m_vertexLayout, m_wrappedDevice, m_graphicsQueue);
        m_objects.m_models.push_back(model);
    }
}

void PbrRenderer::SetupUniformBuffers()
{
    m_wrappedDevice->CreateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                                  &m_uniformBuffers.m_object,
                                  sizeof(UboMatrices));

    m_wrappedDevice->CreateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                                  &m_uniformBuffers.m_params,
                                  sizeof(UboParameters));

    m_uniformBuffers.m_object.Map();
    m_uniformBuffers.m_params.Map();


}

void PbrRenderer::BuildCommandBuffers()
{

}

void PbrRenderer::Render()
{

}

void PbrRenderer::UpdateUniformBuffers()
{
}





