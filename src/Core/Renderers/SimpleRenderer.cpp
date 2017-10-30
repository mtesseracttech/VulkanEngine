//
// Created by MTesseracT on 30-10-2017.
//

#include "SimpleRenderer.hpp"

SimpleRenderer::SimpleRenderer()
{

}

SimpleRenderer::~SimpleRenderer()
{
    m_logicalDevice.destroyPipeline(m_phong);
    m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
    m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);

    m_models.bunny.Destroy();
    m_models.monkey.Destroy();
    m_models.teapot.Destroy();

    m_uniformBuffer.Destroy();
}

void SimpleRenderer::Render()
{

}

void SimpleRenderer::GetEnabledFeatures()
{
    //Not needed for now, but will be needed for wireframe
}


