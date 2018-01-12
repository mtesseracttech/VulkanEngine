//
// Created by MTesseracT on 30-10-2017.
//

#include "SimpleRenderer.hpp"

void SimpleRenderer::Prepare()
{
    VulkanRendererBase::Prepare();
    SetupCamera();
    LoadTextures();
    LoadModels();
    SetupVertexDescriptions();
    PrepareUniformBuffers();
    SetupDescriptorSetLayout();
    PreparePipeline();
    SetupDescriptorPool();
    SetupDescriptorSets();
    BuildCommandBuffers();
}

void SimpleRenderer::Cleanup()
{
    Logger::Log("Cleaning up SimpleRenderer");
    m_logicalDevice.destroyPipeline(m_pipelines.skybox);
    m_logicalDevice.destroyPipeline(m_pipelines.centerObject);
    m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
    m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);
    m_models.centerModel.Destroy();
    m_models.skybox.Destroy();
    m_uniformBuffers.centerObject.Destroy();
    m_uniformBuffers.skybox.Destroy();
    VulkanRendererBase::Cleanup();
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
    clearValues[0].color = vk::ClearColorValue(std::array<float,4>{0.1f, 0.1f,0.1f,1.0f});
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);

    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass        = m_renderPass;
    renderPassBeginInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderPassBeginInfo.renderArea.extent = m_swapchain.GetExtent();
    renderPassBeginInfo.clearValueCount   = clearValues.size();
    renderPassBeginInfo.pClearValues      = clearValues.data();

    for (int32_t i = 0; i < m_drawCommandBuffers.size(); ++i)
    {
        renderPassBeginInfo.framebuffer = m_frameBuffers[i];
        m_drawCommandBuffers[i].begin(commandBufferBeginInfo);

        m_drawCommandBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport;
        viewport.width = m_swapchain.GetExtent().width;
        viewport.height = m_swapchain.GetExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        m_drawCommandBuffers[i].setViewport(0, 1, &viewport);

        vk::Rect2D scissor;
        scissor.extent = m_swapchain.GetExtent();
        scissor.offset = vk::Offset2D(0, 0);

        m_drawCommandBuffers[i].setScissor(0, 1, &scissor);

        vk::DeviceSize offsets[1] = {0};

        //Skybox

        m_drawCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1,
                                                   &m_descriptorSets.skybox, 0, nullptr);

        m_drawCommandBuffers[i].bindVertexBuffers(VERTEX_BUFFER_BIND_ID, 1, &m_models.skybox.m_vertexBuffer.m_buffer, offsets);

        m_drawCommandBuffers[i].bindIndexBuffer(m_models.skybox.m_indexBuffer.m_buffer, 0, vk::IndexType::eUint32);

        m_drawCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelines.skybox);

        m_drawCommandBuffers[i].drawIndexed(m_models.skybox.m_indexCount, 1, 0, 0, 0);

        //Center Object

        m_drawCommandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1,
                                                   &m_descriptorSets.centerObject, 0, nullptr);

        m_drawCommandBuffers[i].bindVertexBuffers(VERTEX_BUFFER_BIND_ID, 1, &m_models.centerModel.m_vertexBuffer.m_buffer, offsets);

        m_drawCommandBuffers[i].bindIndexBuffer(m_models.centerModel.m_indexBuffer.m_buffer, 0, vk::IndexType::eUint32);

        m_drawCommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelines.centerObject);

        m_drawCommandBuffers[i].drawIndexed(m_models.centerModel.m_indexCount, 1, 0, 0, 0);

        //TODO: go into world object, into child objects and add them to the renderpass, needs to be redone for every add/removal

        //End render pass

        m_drawCommandBuffers[i].endRenderPass();

        m_drawCommandBuffers[i].end();
    }
}

void SimpleRenderer::LoadModels()
{
    Logger::Log("Loading models!");

    m_models.centerModel.LoadFromFile(Constants::MODEL_PATH + "FireHydrantMesh.obj",
                                 m_vertexLayout,
                                 m_wrappedDevice,
                                 m_graphicsQueue,
                                 1.0f);
    /*
    m_models.centerModel.LoadFromFile(Constants::MODEL_PATH + "Mp7.obj",
                                      m_vertexLayout,
                                      m_wrappedDevice,
                                      m_graphicsQueue,
                                      1.0f);
    */
    m_models.skybox.LoadFromFile(Constants::MODEL_PATH + "cube.obj",
                                 m_vertexLayout,
                                 m_wrappedDevice,
                                 m_graphicsQueue,
                                 0.05f);
}

void SimpleRenderer::PrepareUniformBuffers()
{
    Logger::Log("Preparing Uniform Buffers");

    m_wrappedDevice->CreateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                                  &m_uniformBuffers.centerObject,
                                  sizeof(m_ubo));

    m_wrappedDevice->CreateBuffer(vk::BufferUsageFlagBits::eUniformBuffer,
                                  vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                                  &m_uniformBuffers.skybox,
                                  sizeof(m_ubo));


    m_uniformBuffers.centerObject.Map();
    m_uniformBuffers.skybox.Map();

    //UpdateUniformBuffers();
}

void SimpleRenderer::UpdateUniformBuffers()
{
    //Moving the camera
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_W)){
        m_camera.SetPosition(m_camera.GetPosition() + m_camera.GetForward() * 0.01f);
    }
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_S)){
        m_camera.SetPosition(m_camera.GetPosition() - m_camera.GetForward() * 0.01f);
    }
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_A)){
        m_camera.SetPosition(m_camera.GetPosition() + m_camera.GetRight() * 0.01f);
    }
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_D)){
        m_camera.SetPosition(m_camera.GetPosition() - m_camera.GetRight() * 0.01f);
    }

    if(glfwGetMouseButton(m_window->GetGlfwWindow(), GLFW_MOUSE_BUTTON_1)){

        int screenCenterX = m_swapchain.GetExtent().width /2;
        int screenCenterY = m_swapchain.GetExtent().height/2;
        auto xScale = -((m_window->GetCursorPos().x - screenCenterX) / screenCenterX);
        auto yScale = (m_window->GetCursorPos().y - screenCenterY) / screenCenterY;

        m_camera.SetRotation(m_camera.GetRotation() + glm::vec3(-yScale/10,-xScale/10,0.0f));
    }

    //Scoped statics storing the center object rotation
    static float xRot, yRot;

    //TODO: Finalize key input system using callback
    //Deciding the rotation
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_I)){
        xRot += 0.1f;
    }
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_K)){
        xRot -= 0.1f;
    }
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_J)){
        yRot += 0.1f;
    }
    if(glfwGetKey(m_window->GetGlfwWindow(), GLFW_KEY_L)){
        yRot -= 0.1f;
    }

    //Composing the model matrix
    glm::mat4 modelMatrix (1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(xRot), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(yRot), glm::vec3(0.0f, 1.0f, 0.0f));

    //Loading up the ubo for the center object
    m_ubo.projection = m_camera.GetPerspectiveMat();
    m_ubo.view       = m_camera.GetViewMat();
    m_ubo.model      = modelMatrix;

    //Copy to the buffer
    memcpy(m_uniformBuffers.centerObject.m_mapped, &m_ubo, sizeof(m_ubo));

    //temp store camera position
    glm::vec3 cameraPos = m_camera.GetPosition();

    //Set the position to 0,0,0 so that the cube is always renderer around the camera
    m_camera.SetPosition(glm::vec3(0,0,0));
    m_ubo.model = glm::mat4(1.0f);
    m_ubo.view  = m_camera.GetViewMat();

    memcpy(m_uniformBuffers.skybox.m_mapped, &m_ubo, sizeof(m_ubo));

    //Restore camera position
    m_camera.SetPosition(cameraPos);
}

void SimpleRenderer::SetupDescriptorSetLayout()
{
    Logger::Log("Setting up descriptor set layout");
    std::vector<vk::DescriptorSetLayoutBinding> setLayoutBindings = {
            //UBO binding
            vk::DescriptorSetLayoutBinding(0,
                                           vk::DescriptorType::eUniformBuffer,
                                           1,
                                           vk::ShaderStageFlagBits::eVertex),
            //Reflection Texture binding
            vk::DescriptorSetLayoutBinding(1,
                                           vk::DescriptorType::eCombinedImageSampler,
                                           1,
                                           vk::ShaderStageFlagBits::eFragment),
            //Diffuse Texture binding
            vk::DescriptorSetLayoutBinding(2,
                                           vk::DescriptorType::eCombinedImageSampler,
                                           1,
                                           vk::ShaderStageFlagBits::eFragment)
    };


    vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutCreateInfo.pBindings    = setLayoutBindings.data();

    m_descriptorSetLayout = m_logicalDevice.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

    m_pipelineLayout = m_logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
}

void SimpleRenderer::PreparePipeline()
{
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
    inputAssemblyState.topology             = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyState.flags                = vk::PipelineInputAssemblyStateCreateFlagBits(0);
    inputAssemblyState.primitiveRestartEnable = vk::Bool32(false);

    vk::PipelineRasterizationStateCreateInfo rasterizationState;
    rasterizationState.polygonMode          = vk::PolygonMode::eFill;
    rasterizationState.cullMode             = vk::CullModeFlagBits::eBack;
    rasterizationState.frontFace            = vk::FrontFace::eCounterClockwise;
    rasterizationState.flags                = vk::PipelineRasterizationStateCreateFlagBits(0);
    rasterizationState.depthClampEnable     = vk::Bool32(false);
    rasterizationState.lineWidth            = 1.0f;

    vk::PipelineColorBlendAttachmentState blendAttachmentState;
    blendAttachmentState.colorWriteMask     = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    blendAttachmentState.blendEnable        = vk::Bool32(false);

    vk::PipelineColorBlendStateCreateInfo colorBlendState;
    colorBlendState.attachmentCount         = 1;
    colorBlendState.pAttachments            = &blendAttachmentState;

    vk::PipelineDepthStencilStateCreateInfo depthStencilState;
    depthStencilState.depthTestEnable       = vk::Bool32(false);
    depthStencilState.depthWriteEnable      = vk::Bool32(false);
    depthStencilState.depthCompareOp        = vk::CompareOp::eLessOrEqual;
    depthStencilState.front                 = depthStencilState.back;
    depthStencilState.back.compareOp        = vk::CompareOp::eAlways;

    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount             = 1;
    viewportState.scissorCount              = 1;
    viewportState.flags                     = vk::PipelineViewportStateCreateFlagBits(0);

    vk::PipelineMultisampleStateCreateInfo multisampleState;
    multisampleState.rasterizationSamples   = vk::SampleCountFlagBits::e1;
    multisampleState.flags                  = vk::PipelineMultisampleStateCreateFlagBits(0);

    std::vector<vk::DynamicState> dynamicStateEnables = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor,
            //vk::DynamicState::eLineWidth
    };

    vk::PipelineDynamicStateCreateInfo dynamicState;
    dynamicState.pDynamicStates             = dynamicStateEnables.data();
    dynamicState.dynamicStateCount          = uint32_t(dynamicStateEnables.size());
    dynamicState.flags                      = vk::PipelineDynamicStateCreateFlagBits(0);

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

    //Skybox specific:

    shaderStages[0] = LoadShader(Constants::SHADER_PATH + "skybox.vert.spv", vk::ShaderStageFlagBits::eVertex);
    shaderStages[1] = LoadShader(Constants::SHADER_PATH + "skybox.frag.spv", vk::ShaderStageFlagBits::eFragment);

    vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.layout               = m_pipelineLayout;
    pipelineCreateInfo.renderPass           = m_renderPass;
    pipelineCreateInfo.flags                = vk::PipelineCreateFlagBits(0);
    pipelineCreateInfo.basePipelineIndex    = -1;
    pipelineCreateInfo.basePipelineHandle   = nullptr;
    pipelineCreateInfo.pVertexInputState    = &m_vertexInfo.inputState;
    pipelineCreateInfo.pInputAssemblyState  = &inputAssemblyState;
    pipelineCreateInfo.pRasterizationState  = &rasterizationState;
    pipelineCreateInfo.pColorBlendState     = &colorBlendState;
    pipelineCreateInfo.pMultisampleState    = &multisampleState;
    pipelineCreateInfo.pViewportState       = &viewportState;
    pipelineCreateInfo.pDepthStencilState   = &depthStencilState;
    pipelineCreateInfo.pDynamicState        = &dynamicState;
    pipelineCreateInfo.stageCount           = shaderStages.size();
    pipelineCreateInfo.pStages              = shaderStages.data();

    m_pipelines.skybox = m_logicalDevice.createGraphicsPipeline(m_pipelineCache, pipelineCreateInfo);

    //Center Object specific

    shaderStages[0] = LoadShader(Constants::SHADER_PATH + "reflect.vert.spv", vk::ShaderStageFlagBits::eVertex);
    shaderStages[1] = LoadShader(Constants::SHADER_PATH + "reflect.frag.spv", vk::ShaderStageFlagBits::eFragment);


    depthStencilState.depthWriteEnable  = vk::Bool32(true);
    depthStencilState.depthTestEnable   = vk::Bool32(true);
    rasterizationState.cullMode         = vk::CullModeFlagBits::eFront;

    m_pipelines.centerObject = m_logicalDevice.createGraphicsPipeline(m_pipelineCache, pipelineCreateInfo);
}

void SimpleRenderer::SetupDescriptorPool()
{
    Logger::Log("Setting up descriptor pool");
    std::vector<vk::DescriptorPoolSize> descriptorPoolSizes{
            vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 2),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 2),
            vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 2)
    };

    vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes    = descriptorPoolSizes.data();
    descriptorPoolCreateInfo.maxSets       = 2;

    m_descriptorPool = m_logicalDevice.createDescriptorPool(descriptorPoolCreateInfo);
}

void SimpleRenderer::SetupDescriptorSets()
{
    Logger::Log("Setting up descriptor sets");

    vk::DescriptorImageInfo reflectionDescriptor;
    reflectionDescriptor.imageView   = m_skyboxTex.m_imageView;
    reflectionDescriptor.imageLayout = m_skyboxTex.m_imageLayout;
    reflectionDescriptor.sampler     = m_skyboxTex.m_imageSampler;

    vk::DescriptorImageInfo textureDescriptor;
    textureDescriptor.imageView   = m_centerObjectTex.m_imageView;
    textureDescriptor.imageLayout = m_centerObjectTex.m_imageLayout;
    textureDescriptor.sampler     = m_centerObjectTex.m_imageSampler;

    vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo;
    descriptorSetAllocateInfo.descriptorPool     = m_descriptorPool;
    descriptorSetAllocateInfo.pSetLayouts        = &m_descriptorSetLayout;
    descriptorSetAllocateInfo.descriptorSetCount = 1;

    m_descriptorSets.centerObject = m_logicalDevice.allocateDescriptorSets(descriptorSetAllocateInfo)[0]; //only one is given, so taking the first works

    std::vector<vk::WriteDescriptorSet> writeDescriptorSets{
            vk::WriteDescriptorSet(m_descriptorSets.centerObject, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &m_uniformBuffers.centerObject.m_descriptor, nullptr),
            vk::WriteDescriptorSet(m_descriptorSets.centerObject, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &reflectionDescriptor, nullptr, nullptr),
            vk::WriteDescriptorSet(m_descriptorSets.centerObject, 2, 0, 1, vk::DescriptorType::eCombinedImageSampler, &textureDescriptor, nullptr, nullptr)
    };

    m_logicalDevice.updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

    m_descriptorSets.skybox = m_logicalDevice.allocateDescriptorSets(descriptorSetAllocateInfo)[0];

    writeDescriptorSets = {
            vk::WriteDescriptorSet(m_descriptorSets.skybox, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &m_uniformBuffers.skybox.m_descriptor, nullptr),
            vk::WriteDescriptorSet(m_descriptorSets.skybox, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &reflectionDescriptor, nullptr, nullptr)
    };

    m_logicalDevice.updateDescriptorSets(static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
}

void SimpleRenderer::Render()
{
    //Prepares a swap chain image
    VulkanRendererBase::PrepareFrame();

    m_submitInfo.commandBufferCount = 1;
    m_submitInfo.pCommandBuffers = &m_drawCommandBuffers[m_currentBuffer];

    m_graphicsQueue.submit(1, &m_submitInfo, nullptr);

    VulkanRendererBase::SubmitFrame();
}

void SimpleRenderer::LoadTextures()
{
    //Check if the device supports BC hardware decompression
    if(m_deviceFeatures.textureCompressionBC)
    {
        m_skyboxTex.LoadFromFile(m_wrappedDevice, Constants::TEXTURE_PATH + "cubemap_yokohama_bc3_unorm.ktx", vk::Format::eBc3UnormBlock, m_graphicsQueue);
        //m_skyboxTex.LoadFromFile(m_wrappedDevice, Constants::TEXTURE_PATH + "NebulaSkyboxBC3.ktx", vk::Format::eBc3SrgbBlock, m_graphicsQueue);
        m_centerObjectTex.LoadFromFile(m_wrappedDevice, Constants::TEXTURE_PATH + "FireHydrant/fire_hydrant_Base_Color_BC3.ktx", vk::Format::eBc3SrgbBlock, m_graphicsQueue);
        //m_centerObjectTex.LoadFromFile(m_wrappedDevice, Constants::TEXTURE_PATH + "Mp7/MP7_D.ktx", vk::Format::eBc3SrgbBlock, m_graphicsQueue);
    }
}

void SimpleRenderer::CreateCubemap(const std::string &p_filename, vk::Format p_format)
{
    Logger::Log("Loading skybox: " + p_filename);
}

void SimpleRenderer::SetupVertexDescriptions()
{
    //Single vertex layout is needed since loaded models will be forced to have this layout through m_vertexLayout
    m_vertexInfo.bindingDescriptions.resize(1);

    m_vertexInfo.bindingDescriptions[0].binding     = VERTEX_BUFFER_BIND_ID;
    m_vertexInfo.bindingDescriptions[0].stride      = m_vertexLayout.GetStride();
    m_vertexInfo.bindingDescriptions[0].inputRate   = vk::VertexInputRate::eVertex;

    //Vertex layout contains 3 components
    m_vertexInfo.attributeDescriptions.resize(3);

    //Position
    m_vertexInfo.attributeDescriptions[0].binding   = VERTEX_BUFFER_BIND_ID;
    m_vertexInfo.attributeDescriptions[0].format    = vk::Format::eR32G32B32Sfloat;
    m_vertexInfo.attributeDescriptions[0].location  = 0;
    m_vertexInfo.attributeDescriptions[0].offset    = 0;

    //Normal
    m_vertexInfo.attributeDescriptions[1].binding   = VERTEX_BUFFER_BIND_ID;
    m_vertexInfo.attributeDescriptions[1].format    = vk::Format::eR32G32B32Sfloat;
    m_vertexInfo.attributeDescriptions[1].location  = 1;
    m_vertexInfo.attributeDescriptions[1].offset    = sizeof(float) * 3;

    //UVs
    m_vertexInfo.attributeDescriptions[2].binding   = VERTEX_BUFFER_BIND_ID;
    m_vertexInfo.attributeDescriptions[2].format    = vk::Format::eR32G32Sfloat;
    m_vertexInfo.attributeDescriptions[2].location  = 2;
    m_vertexInfo.attributeDescriptions[2].offset    = sizeof(float) * 6;

    //Creating the input state
    m_vertexInfo.inputState.vertexBindingDescriptionCount   = static_cast<uint32_t>(m_vertexInfo.bindingDescriptions.size());
    m_vertexInfo.inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_vertexInfo.attributeDescriptions.size());
    m_vertexInfo.inputState.pVertexBindingDescriptions      = m_vertexInfo.bindingDescriptions.data();
    m_vertexInfo.inputState.pVertexAttributeDescriptions    = m_vertexInfo.attributeDescriptions.data();
}

void SimpleRenderer::SetupCamera()
{
    m_camera.SetPerspective(60, (static_cast<float>(m_window->GetWindowSize().x) / static_cast<float>(m_window->GetWindowSize().y)), 0.01f, 256.0f);
    m_camera.SetCameraType(CameraType::FirstPerson);
    m_camera.SetPosition(glm::vec3(0,0,0));
    m_camera.SetRotation(glm::vec3(-25.0f, 15.0f, 0.0f));
}

void SimpleRenderer::WindowResized()
{
    m_camera.SetPerspective(60, (static_cast<float>(m_window->GetWindowSize().x) / static_cast<float>(m_window->GetWindowSize().y)), 0.01f, 256.0f);
}