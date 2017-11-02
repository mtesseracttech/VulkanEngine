//
// Created by MTesseracT on 30-10-2017.
//

#include "SimpleRenderer.hpp"

SimpleRenderer::SimpleRenderer()
{

}

SimpleRenderer::~SimpleRenderer()
{
    m_logicalDevice.destroyPipeline(m_graphicsPipeline);
    m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
    m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);
    m_models.teapot.Destroy();
    m_uniformBuffer.Destroy();
}

void SimpleRenderer::Prepare()
{
    VulkanRendererBase::Prepare();
    LoadModels();
    PrepareUniformBuffers();
    SetupDescriptorSetLayout();
    PreparePipeline();
    SetupDescriptorPool();
    SetupDescriptorSet();
    BuildCommandBuffers();
}

void SimpleRenderer::Render()
{
    DrawFrame();
}

void SimpleRenderer::GetEnabledFeatures()
{
}

void SimpleRenderer::BuildCommandBuffers()
{
    Logger::Log("Building Command Buffers");
    vk::CommandBufferBeginInfo commandBufferBeginInfo;
    //commandBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits ::eSimultaneousUse;

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color = m_defaultClearColor;
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);

    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderPassBeginInfo.renderArea.extent = m_swapchain.GetExtent();
    renderPassBeginInfo.clearValueCount = clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();

    for (int32_t i = 0; i < m_drawCommandBuffers.size(); ++i)
    {
        renderPassBeginInfo.framebuffer = m_frameBuffers[i];
        m_drawCommandBuffers[i].begin(commandBufferBeginInfo);

        m_drawCommandBuffers[i].beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        //std::cout << m_swapchain.GetExtent().width << ", " << m_swapchain.GetExtent().height << std::endl;

        vk::Viewport viewport;
        viewport.x = 0.0;
        viewport.y = 0.0;
        viewport.width = m_swapchain.GetExtent().width;
        viewport.height = m_swapchain.GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        m_drawCommandBuffers[i].setViewport(0, 1, &viewport);

        vk::Rect2D scissor;
        scissor.extent = m_swapchain.GetExtent();
        scissor.offset = vk::Offset2D(0, 0);

        m_drawCommandBuffers[i].setScissor(0, 1, &scissor);

        m_drawCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1,
                                                   &m_descriptorSet, 0, nullptr);

        vk::DeviceSize offsets[1] = {0};

        m_drawCommandBuffers[i].bindVertexBuffers(0, 1, &m_models.teapot.m_vertexBuffer.m_buffer, offsets);

        m_drawCommandBuffers[i].bindIndexBuffer(m_models.teapot.m_indexBuffer.m_buffer, 0, vk::IndexType::eUint32);

        m_drawCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphicsPipeline);

        m_drawCommandBuffers[i].drawIndexed(m_models.teapot.m_indexCount, 1, 0, 0, 0);

        m_drawCommandBuffers[i].endRenderPass();

        m_drawCommandBuffers[i].end();
    }
}

void SimpleRenderer::LoadModels()
{
    Logger::Log("Loading models!");
    m_models.teapot.LoadFromFile(Constants::MODEL_PATH + "treasure_smooth.dae",
                                 m_vertexLayout,
                                 m_wrappedDevice,
                                 m_graphicsQueue);
}

void SimpleRenderer::PrepareUniformBuffers()
{
    Logger::Log("Preparing Uniform Buffers");
    m_wrappedDevice->CreateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                                  &m_uniformBuffer,
                                  sizeof(m_ubo));
    m_uniformBuffer.Map();

    UpdateUniformBuffers();
}

void SimpleRenderer::UpdateUniformBuffers()
{
    m_ubo.projection = glm::perspective(glm::radians(60.0f), (float) (m_swapchain.GetExtent().width / 3.0f) /
                                                             (float) m_swapchain.GetExtent().height, 0.1f, 256.0f);

    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    m_ubo.modelView = viewMatrix * glm::translate(glm::mat4(1.0f), m_cameraPosition);
    m_ubo.modelView = glm::rotate(m_ubo.modelView, glm::radians(m_cameraRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_ubo.modelView = glm::rotate(m_ubo.modelView, glm::radians(m_cameraRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_ubo.modelView = glm::rotate(m_ubo.modelView, glm::radians(m_cameraRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    memcpy(m_uniformBuffer.m_mapped, &m_ubo, sizeof(m_ubo));
}

void SimpleRenderer::SetupDescriptorSetLayout()
{
    Logger::Log("Setting up descriptor set layout");
    vk::DescriptorSetLayoutBinding vertexShaderBufferBinding;
    vertexShaderBufferBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    vertexShaderBufferBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
    vertexShaderBufferBinding.binding = 0;
    vertexShaderBufferBinding.descriptorCount = 1;

    std::vector<vk::DescriptorSetLayoutBinding> setLayoutBindings = {vertexShaderBufferBinding};

    vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutCreateInfo.pBindings = setLayoutBindings.data();

    m_descriptorSetLayout = m_logicalDevice.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

    m_pipelineLayout = m_logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
}

void SimpleRenderer::PreparePipeline()
{
    Logger::Log("Preparing phong pipeline");
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
    inputAssemblyState.topology         = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyState.flags            = static_cast<vk::PipelineInputAssemblyStateCreateFlagBits >(0);
    inputAssemblyState.primitiveRestartEnable = static_cast<vk::Bool32>(false);

    vk::PipelineRasterizationStateCreateInfo rasterizationState;
    rasterizationState.polygonMode      = vk::PolygonMode::eFill;
    rasterizationState.cullMode         = vk::CullModeFlagBits::eBack;
    rasterizationState.frontFace        = vk::FrontFace::eClockwise;
    rasterizationState.flags            = static_cast<vk::PipelineRasterizationStateCreateFlagBits>(0);
    rasterizationState.depthClampEnable = static_cast<vk::Bool32>(false);
    rasterizationState.lineWidth        = 1.0f;

    vk::PipelineColorBlendAttachmentState blendAttachmentState;
    blendAttachmentState.colorWriteMask = static_cast<vk::ColorComponentFlagBits >(0xf);
    blendAttachmentState.blendEnable    = static_cast<vk::Bool32>(false);

    vk::PipelineColorBlendStateCreateInfo colorBlendState;
    colorBlendState.attachmentCount     = 1;
    colorBlendState.pAttachments        = &blendAttachmentState;

    vk::PipelineDepthStencilStateCreateInfo depthStencilState;
    depthStencilState.depthTestEnable   = static_cast<vk::Bool32>(true);
    depthStencilState.depthWriteEnable  = static_cast<vk::Bool32>(true);
    depthStencilState.depthCompareOp    = vk::CompareOp::eLessOrEqual;
    depthStencilState.front             = depthStencilState.back;
    depthStencilState.back.compareOp    = vk::CompareOp::eAlways;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.flags = static_cast<vk::PipelineViewportStateCreateFlagBits>(0);

    vk::PipelineMultisampleStateCreateInfo multisampleState;
    multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampleState.flags = static_cast<vk::PipelineMultisampleStateCreateFlagBits >(0);

    std::vector<vk::DynamicState> dynamicStateEnables = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
            vk::DynamicState::eLineWidth
    };

    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.pDynamicStates = dynamicStateEnables.data();
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
    dynamicState.flags = static_cast<vk::PipelineDynamicStateCreateFlagBits>(0);

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

    vk::VertexInputBindingDescription vertexInputBindingDescription;
    vertexInputBindingDescription.binding = 0;
    vertexInputBindingDescription.stride = m_vertexLayout.GetStride();
    vertexInputBindingDescription.inputRate = vk::VertexInputRate::eVertex;

    std::vector<vk::VertexInputBindingDescription> vertexInputBindings = {
            vertexInputBindingDescription
    };

    vk::VertexInputAttributeDescription positionAttribute;
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = vk::Format::eR32G32B32Sfloat;
    positionAttribute.offset = 0;

    vk::VertexInputAttributeDescription normalAttribute;
    normalAttribute.binding = 0;
    normalAttribute.location = 1;
    normalAttribute.format = vk::Format::eR32G32B32Sfloat;
    normalAttribute.offset = sizeof(float) * 3; //Size of the posAttribute

    vk::VertexInputAttributeDescription uvAttribute;
    uvAttribute.binding = 0;
    uvAttribute.location = 2;
    uvAttribute.format = vk::Format::eR32G32Sfloat;
    uvAttribute.offset = sizeof(float) * 6;

    vk::VertexInputAttributeDescription colorAttribute;
    colorAttribute.binding = 0;
    colorAttribute.location = 3;
    colorAttribute.format = vk::Format::eR32G32B32Sfloat;
    colorAttribute.offset = sizeof(float) * 8;

    std::vector<vk::VertexInputAttributeDescription> vertexInputAttributes = {
            positionAttribute,
            normalAttribute,
            uvAttribute,
            colorAttribute
    };

    vk::PipelineVertexInputStateCreateInfo vertexInputState;
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
    vertexInputState.pVertexBindingDescriptions = vertexInputBindings.data();
    vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

    shaderStages[0] = LoadShader(Constants::SHADER_PATH + "vert.spv", vk::ShaderStageFlagBits::eVertex);
    shaderStages[1] = LoadShader(Constants::SHADER_PATH + "frag.spv", vk::ShaderStageFlagBits::eFragment);

    vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.layout = m_pipelineLayout;
    pipelineCreateInfo.renderPass = m_renderPass;
    pipelineCreateInfo.flags = static_cast<vk::PipelineCreateFlagBits>(0);
    pipelineCreateInfo.basePipelineIndex = -1;
    pipelineCreateInfo.basePipelineHandle = nullptr;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.stageCount = shaderStages.size();
    pipelineCreateInfo.pStages = shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputState;

    m_graphicsPipeline = m_logicalDevice.createGraphicsPipeline(m_pipelineCache, pipelineCreateInfo);
}

void SimpleRenderer::SetupDescriptorPool()
{
    Logger::Log("Setting up descriptor pool");
    vk::DescriptorPoolSize descriptorPoolSize;
    descriptorPoolSize.type = vk::DescriptorType::eUniformBuffer;
    descriptorPoolSize.descriptorCount = 1;

    std::vector<vk::DescriptorPoolSize> poolSizes = {
            descriptorPoolSize
    };

    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
    descriptorPoolCreateInfo.maxSets = 2;

    m_descriptorPool = m_logicalDevice.createDescriptorPool(descriptorPoolCreateInfo);
}

void SimpleRenderer::SetupDescriptorSet()
{
    Logger::Log("Setting up descriptor set");
    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
    descriptorSetAllocateInfo.pSetLayouts = &m_descriptorSetLayout;
    descriptorSetAllocateInfo.descriptorSetCount = 1;

    m_descriptorSet = m_logicalDevice.allocateDescriptorSets(
            descriptorSetAllocateInfo)[0]; //only one is given, so taking the first works

    vk::WriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.dstSet = m_descriptorSet;
    writeDescriptorSet.descriptorType = vk::DescriptorType::eUniformBuffer;
    writeDescriptorSet.dstBinding = 0;
    writeDescriptorSet.pBufferInfo = &m_uniformBuffer.m_descriptor;
    writeDescriptorSet.descriptorCount = 1;

    std::vector<vk::WriteDescriptorSet> writeDescriptorSets = {
            writeDescriptorSet
    };

    m_logicalDevice.updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(),
                                         0, nullptr);
}

void SimpleRenderer::DrawFrame()
{
    VulkanRendererBase::PrepareFrame();

    static int slowFrameCounter = 0;
    if (++slowFrameCounter % 1000 == 0) std::cout << "rendered 1000 frames" << std::endl;

    m_submitInfo.commandBufferCount = 1;
    m_submitInfo.pCommandBuffers = &m_drawCommandBuffers[m_currentBuffer];

    m_graphicsQueue.submit(1, &m_submitInfo, nullptr);

    VulkanRendererBase::SubmitFrame();
}
