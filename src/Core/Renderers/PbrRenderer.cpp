//
// Created by mtesseract on 20-12-17.
//

#include <Core/MtVulkanAPI/VertexInputDescriptor.hpp>
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
    //Preparing Descriptor Set Layout
    SetupDescriptorSetLayout();
    //Preparing the pipeline layout
    SetupPipelineLayout();
    //Setting up the graphics pipeline
    SetupPipeline();
    //Setting up the Descriptor Sets
    SetupDescriptorSets();
}

void PbrRenderer::Cleanup()
{
    //Renderer Specific Cleanup Code First
    m_logicalDevice.destroyPipeline(m_pipeline);
    m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
    m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);

    for (auto &model : m_objects.m_models)
    {
        model.Destroy();
    }

    if (m_uniformBuffers.m_object.m_device)
    {
        m_uniformBuffers.m_object.Destroy();
    }

    if (m_uniformBuffers.m_params.m_device)
    {
        m_uniformBuffers.m_params.Destroy();
    }

    //Base Renderer Cleanup Second
    VulkanRendererBase::Cleanup();
}

void PbrRenderer::SetupCamera()
{
    m_camera.SetPerspective(90.0f, m_window->GetAspectRatio(), 0.001f, 1000.0f);
    m_camera.SetCameraType(CameraType::FirstPerson);
    std::cout << m_window->GetAspectRatio() << std::endl;
    m_camera.SetPosition(glm::vec3(10.0f, 13.0f, 1.8f));
    m_camera.SetRotation(glm::vec3(-62.5f, 90.0f, 0.0f));
}

void PbrRenderer::SetupMaterials()
{
    m_materials.clear();
    //Metals
    m_materials.push_back(SimplePbrMaterial(glm::vec3(1.0f, 0.765557f, 0.336057f), 0.1f, 1.0f));
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
    std::vector<std::string> filenames = {"teapot.dae"};
    for (const auto          &file : filenames)
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

void PbrRenderer::SetupDescriptorSetLayout()
{
    vk::DescriptorSetLayoutBinding uboMatrixLayoutBinding;
    uboMatrixLayoutBinding.descriptorType  = vk::DescriptorType::eUniformBuffer;
    uboMatrixLayoutBinding.stageFlags      = vk::ShaderStageFlagBits::eVertex;
    uboMatrixLayoutBinding.descriptorCount = 1;
    uboMatrixLayoutBinding.binding         = 0;

    vk::DescriptorSetLayoutBinding uboPropertiesLayoutBinding;
    uboPropertiesLayoutBinding.descriptorType  = vk::DescriptorType::eUniformBuffer;
    uboPropertiesLayoutBinding.stageFlags      = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
    uboPropertiesLayoutBinding.descriptorCount = 1;
    uboPropertiesLayoutBinding.binding         = 1;

    std::vector<vk::DescriptorSetLayoutBinding> layoutBindings = {
            uboMatrixLayoutBinding,
            uboPropertiesLayoutBinding
    };

    vk::DescriptorSetLayoutCreateInfo createInfo;
    createInfo.pBindings    = layoutBindings.data();
    createInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());

    m_descriptorSetLayout = m_logicalDevice.createDescriptorSetLayout(createInfo);
}

void PbrRenderer::SetupPipelineLayout()
{
    vk::PushConstantRange objectPositionConstantRange;
    objectPositionConstantRange.size       = sizeof(glm::vec3);
    objectPositionConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex;

    vk::PushConstantRange materialPropertiesConstantRange;
    materialPropertiesConstantRange.size       = sizeof(SimplePbrMaterial::Properties);
    materialPropertiesConstantRange.stageFlags = vk::ShaderStageFlagBits::eFragment;

    std::vector<vk::PushConstantRange> pushConstantRanges = {
            objectPositionConstantRange,
            materialPropertiesConstantRange
    };

    vk::PipelineLayoutCreateInfo layoutCreateInfo;
    layoutCreateInfo.pSetLayouts            = &m_descriptorSetLayout;
    layoutCreateInfo.setLayoutCount         = 1;
    layoutCreateInfo.pPushConstantRanges    = pushConstantRanges.data();
    layoutCreateInfo.pushConstantRangeCount = 2;

    m_pipelineLayout = m_logicalDevice.createPipelineLayout(layoutCreateInfo);
}

void PbrRenderer::SetupPipeline()
{
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
    inputAssemblyState.primitiveRestartEnable = vk::Bool32(false);
    inputAssemblyState.topology               = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyState.flags                  = vk::PipelineInputAssemblyStateCreateFlagBits(0);

    vk::PipelineRasterizationStateCreateInfo rasterizationState;
    rasterizationState.cullMode    = vk::CullModeFlagBits::eFront;
    rasterizationState.polygonMode = vk::PolygonMode::eFill;
    rasterizationState.frontFace   = vk::FrontFace::eCounterClockwise;

    vk::PipelineColorBlendAttachmentState colorBlendAttachmentState;
    colorBlendAttachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR |
                                               vk::ColorComponentFlagBits::eG |
                                               vk::ColorComponentFlagBits::eB |
                                               vk::ColorComponentFlagBits::eA;
    colorBlendAttachmentState.blendEnable    = vk::Bool32(false);

    vk::PipelineColorBlendStateCreateInfo colorBlendState;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments    = &colorBlendAttachmentState;

    vk::PipelineDepthStencilStateCreateInfo depthStencilState;
    depthStencilState.depthTestEnable  = vk::Bool32(true);
    depthStencilState.depthWriteEnable = vk::Bool32(true);
    depthStencilState.depthCompareOp   = vk::CompareOp::eLessOrEqual;
    depthStencilState.front            = depthStencilState.back;
    depthStencilState.back.compareOp   = vk::CompareOp::eAlways;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.flags         = vk::PipelineViewportStateCreateFlagBits(0);
    viewportState.scissorCount  = 1;
    viewportState.viewportCount = 1;

    vk::PipelineMultisampleStateCreateInfo multiSampleState;
    multiSampleState.flags                = vk::PipelineMultisampleStateCreateFlagBits(0);
    multiSampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;

    std::vector<vk::DynamicState> dynamicStateList = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
    };

    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateList.size());
    dynamicState.pDynamicStates    = dynamicStateList.data();
    dynamicState.flags             = vk::PipelineDynamicStateCreateFlagBits(0);

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0] = LoadShader(Constants::SHADER_PATH + "pbr.vert.spv", vk::ShaderStageFlagBits::eVertex);
    shaderStages[1] = LoadShader(Constants::SHADER_PATH + "pbr.frag.spv", vk::ShaderStageFlagBits::eFragment);

    VertexInputDescriptor inputDescriptor(m_vertexLayout);

    vk::PipelineVertexInputStateCreateInfo vertexInputState;
    vertexInputState.vertexBindingDescriptionCount   = inputDescriptor.GetBindingCount();
    vertexInputState.pVertexBindingDescriptions      = inputDescriptor.GetBindings();
    vertexInputState.vertexAttributeDescriptionCount = inputDescriptor.GetAttributeCount();
    vertexInputState.pVertexAttributeDescriptions    = inputDescriptor.GetAttributes();

    vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.layout              = m_pipelineLayout;
    pipelineCreateInfo.renderPass          = m_renderPass;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState    = &colorBlendState;
    pipelineCreateInfo.pMultisampleState   = &multiSampleState;
    pipelineCreateInfo.pViewportState      = &viewportState;
    pipelineCreateInfo.pDepthStencilState  = &depthStencilState;
    pipelineCreateInfo.pDynamicState       = &dynamicState;
    pipelineCreateInfo.stageCount          = static_cast<uint32_t>(shaderStages.size());
    pipelineCreateInfo.pStages             = shaderStages.data();
    pipelineCreateInfo.pVertexInputState   = &vertexInputState;

    m_pipeline = m_logicalDevice.createGraphicsPipeline(m_pipelineCache, pipelineCreateInfo);
}

void PbrRenderer::SetupDescriptorSets()
{
    vk::DescriptorPoolSize descriptorPoolSize;
    descriptorPoolSize.descriptorCount = 4;
    descriptorPoolSize.type            = vk::DescriptorType::eUniformBuffer;

    std::vector<vk::DescriptorPoolSize> poolSizes = {descriptorPoolSize};

    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.pPoolSizes    = poolSizes.data();
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolCreateInfo.maxSets       = 2;

    m_descriptorPool = m_logicalDevice.createDescriptorPool(descriptorPoolCreateInfo);

    vk::DescriptorSetAllocateInfo allocateInfo;
    allocateInfo.descriptorPool     = m_descriptorPool;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts        = &m_descriptorSetLayout;

    m_descriptorSet = m_logicalDevice.allocateDescriptorSets(allocateInfo)[0];

    vk::WriteDescriptorSet objectWriteDescriptorSet;
    objectWriteDescriptorSet.dstSet          = m_descriptorSet;
    objectWriteDescriptorSet.descriptorType  = vk::DescriptorType::eUniformBuffer;
    objectWriteDescriptorSet.dstBinding      = 0;
    objectWriteDescriptorSet.descriptorCount = 1;
    objectWriteDescriptorSet.pBufferInfo     = &m_uniformBuffers.m_object.m_descriptor;

    vk::WriteDescriptorSet parameterWriteDescriptorSet;
    parameterWriteDescriptorSet.dstSet          = m_descriptorSet;
    parameterWriteDescriptorSet.descriptorType  = vk::DescriptorType::eUniformBuffer;
    parameterWriteDescriptorSet.dstBinding      = 1;
    parameterWriteDescriptorSet.descriptorCount = 1;
    parameterWriteDescriptorSet.pBufferInfo     = &m_uniformBuffers.m_params.m_descriptor;

    std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
            objectWriteDescriptorSet,
            parameterWriteDescriptorSet
    };

    m_logicalDevice.updateDescriptorSets(writeDescriptorSets, nullptr);
}

void PbrRenderer::BuildCommandBuffer()
{
    std::array<vk::ClearValue, 2> clearValues = {};
    clearValues[0].color        = vk::ClearColorValue({0.0f,0.0f,0.0f,1.0f});
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);

    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass        = m_renderPass;
    renderPassBeginInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderPassBeginInfo.renderArea.extent = m_swapchain.GetExtent();
    renderPassBeginInfo.clearValueCount   = clearValues.size();
    renderPassBeginInfo.pClearValues      = clearValues.data();

    vk::CommandBufferBeginInfo commandBufferBeginInfo;

    for (uint32_t i = 0; i < m_drawCommandBuffers.size(); ++i)
    {
        renderPassBeginInfo.framebuffer = m_frameBuffers[i];

        m_drawCommandBuffers[i].begin(commandBufferBeginInfo);

        m_drawCommandBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport;
        viewport.height   = m_swapchain.GetExtent().height;
        viewport.width    = m_swapchain.GetExtent().width;
        viewport.maxDepth = 1.0f;
        viewport.minDepth = 0.0f;

        m_drawCommandBuffers[i].setViewport(0, 1, &viewport);

        vk::Rect2D scissor;
        scissor.extent = m_swapchain.GetExtent();
        scissor.offset = vk::Offset2D(0, 0);

        m_drawCommandBuffers[i].setScissor(0, 1, &scissor);

        std::array<vk::DeviceSize, 1> offsets = { 0 };

        m_drawCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, &m_pipeline);

        m_drawCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);

        m_drawCommandBuffers[i].bindVertexBuffers(VERTEX_BUFFER_BIND_ID, 1, &m_objects.m_models[m_objects.m_objectIndex].m_vertexBuffer.m_buffer, offsets.data());

        m_drawCommandBuffers[i].bindIndexBuffer(&m_objects.m_models[m_objects.m_objectIndex].m_indexBuffer.m_buffer, 0, vk::IndexType::eUint32);

        SimplePbrMaterial material = m_materials[m_materialIndex];

        material.m_props.metallicness = 1.0f;
        material.m_props.roughness = 1.0f;

        glm::vec3 pos = glm::vec3(0,0,0);

        m_drawCommandBuffers[i].pushConstants(m_pipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::vec3), &pos);
        m_drawCommandBuffers[i].pushConstants(m_pipelineLayout, vk::ShaderStageFlagBits::eFragment, sizeof(glm::vec3), sizeof(SimplePbrMaterial::Properties), &material);

        m_drawCommandBuffers[i].endRenderPass();

        m_drawCommandBuffers[i].end();
    }
}

