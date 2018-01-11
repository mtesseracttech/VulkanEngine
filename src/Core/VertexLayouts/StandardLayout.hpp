//
// Created by mtesseract on 21-12-17.
//

#ifndef VULKANENGINE_STANDARDLAYOUT_HPP
#define VULKANENGINE_STANDARDLAYOUT_HPP

#include <Core/MtVulkanAPI/VulkanModel.hpp>

struct StandardLayout{
    static const VertexLayout basicLayout = VertexLayout({
        VulkanVertexComponent::ePosition,
        VulkanVertexComponent::eNormal,
        VulkanVertexComponent::eUV
    });

    static const std::vector<vk::VertexInputAttributeDescription> vertexInputAttributes = {
            //(Location, binding, format, offset)
            vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, sizeof(float) * 0), //Position
            vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, sizeof(float) * 3), //Normal
            vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat   , sizeof(float) * 5)  //UV
    };
};

#endif //VULKANENGINE_STANDARDLAYOUT_HPP
