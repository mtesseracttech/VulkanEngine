//
// Created by MTesseracT on 21-10-2017.
//

#include "VulkanHelpers.hpp"

vk::Bool32 VulkanHelpers::GetSupportedDepthFormat(vk::PhysicalDevice p_physicalDevice, vk::Format *p_depthFormat)
{
    std::vector<vk::Format> depthFormats = {
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD32Sfloat,
            vk::Format::eD24UnormS8Uint,
            vk::Format::eD16UnormS8Uint,
            vk::Format::eD16Unorm
    };

    for (auto& format : depthFormats)
    {
        vk::FormatProperties formatProps = p_physicalDevice.getFormatProperties(format);

        if(formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
        {
            *p_depthFormat = format;
            return static_cast<vk::Bool32>(true);
        }
    }

    return static_cast<vk::Bool32>(false);
}
