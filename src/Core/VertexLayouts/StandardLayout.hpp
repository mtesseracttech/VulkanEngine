//
// Created by mtesseract on 21-12-17.
//

#ifndef VULKANENGINE_STANDARDLAYOUT_HPP
#define VULKANENGINE_STANDARDLAYOUT_HPP

#include <Core/MtVulkanAPI/VulkanModel.hpp>

struct StandardLayout{
    const VertexLayout basicLayout = VertexLayout({
        VulkanVertexComponent::ePosition,
        VulkanVertexComponent::eNormal,
        VulkanVertexComponent::eUV
    });

    std::vector<vk::VertexInputAttributeDescription> vertexInputAttributes = {
            vk::VertexInputAttributeDescription()
            //vks::initializers::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),					// Position
            //vks::initializers::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3),	// Normal
            //vks::initializers::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, sizeof(float) * 5),		// UV
    };
};

#endif //VULKANENGINE_STANDARDLAYOUT_HPP
