//
// Created by mtesseract on 8-2-18.
//

#ifndef VULKANENGINE_IRENDERABLE_HPP
#define VULKANENGINE_IRENDERABLE_HPP

#include <vulkan/vulkan.hpp>
#include <Material/AbstractMaterial.hpp>
#include "VulkanModel.hpp"

class IRenderable
{
protected:
    AbstractMaterial *m_material = nullptr;
    VulkanModel      *m_model    = nullptr;
public:
    void BuildCommandBuffer(vk::CommandBuffer p_cmdBuffer);

    void SetMaterial(AbstractMaterial * p_material);

    void SetModel(VulkanModel * p_model);

    //Warning: do not call during the renderpass
    void UpdateUniforms();
};


#endif //VULKANENGINE_IRENDERABLE_HPP
