//
// Created by MTesseracT on 23-10-2017.
//

#ifndef VULKANENGINE_VULKANTEXTURE_HPP
#define VULKANENGINE_VULKANTEXTURE_HPP

#include <sstream>
#include "Core/MtVulkanAPI/Device/VulkanDevice.hpp"
#include "VulkanHelpers.hpp"
#include <gli/gli.hpp>

struct VulkanBaseTexture
{
    vk::Device              m_device;
    vk::Image               m_image;
    vk::ImageLayout         m_imageLayout;
    vk::DeviceMemory        m_deviceMemory;
    vk::ImageView           m_imageView;
    vk::DescriptorImageInfo m_descriptorInfo;
    vk::Sampler             m_imageSampler;
    uint32_t                m_width;
    uint32_t                m_height;
    uint32_t                m_mipLevels;
    uint32_t                m_layers;

    void UpdateDescriptor()
    {
        m_descriptorInfo.sampler     = m_imageSampler;
        m_descriptorInfo.imageLayout = m_imageLayout;
        m_descriptorInfo.imageView   = m_imageView;
    }

public:

    void Destroy()
    {
        m_device.destroyImageView(m_imageView);
        m_device.destroyImage(m_image);
        if (m_imageSampler)
        {
            m_device.destroySampler(m_imageSampler);
        }
        m_device.freeMemory(m_deviceMemory);
    }
};

struct VulkanTexture2D : VulkanBaseTexture
{
    void LoadFromFile(
            VulkanDevice *p_device,
            std::string p_filename,
            vk::Format p_format,
            vk::Queue p_copyQueue,
            vk::ImageUsageFlags p_imageUsageFlags = vk::ImageUsageFlagBits::eSampled,
            vk::ImageLayout p_imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
            bool p_forceLinear = false)
    {
        gli::texture2d texture(gli::load(p_filename.c_str()));

        if (texture.empty())
        {
            std::stringstream errorMessage;
            errorMessage << "Could not load " << p_filename.c_str();
            throw std::runtime_error(errorMessage.str());
        }

        m_device = p_device->m_logicalDevice;

        m_width     = static_cast<uint32_t>(texture[0].extent().x);
        m_height    = static_cast<uint32_t>(texture[0].extent().y);
        m_mipLevels = static_cast<uint32_t>(texture.levels());
        m_layers    = static_cast<uint32_t>(texture.layers());

        vk::FormatProperties formatProperties;
        p_device->m_physicalDevice.getFormatProperties(p_format, &formatProperties);

        auto useStaging = static_cast<vk::Bool32>(!p_forceLinear);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        vk::MemoryRequirements memoryRequirements;

        vk::CommandBuffer copyCommandBuffer = p_device->CreateCommandBuffer(vk::CommandBufferLevel::ePrimary, true);

        if (useStaging)
        {
            vk::BufferCreateInfo bufferCreateInfo;
            bufferCreateInfo.size        = texture.size();
            bufferCreateInfo.usage       = vk::BufferUsageFlagBits::eTransferSrc;
            bufferCreateInfo.sharingMode = vk::SharingMode::eExclusive;

            vk::Buffer stagingBuffer = m_device.createBuffer(bufferCreateInfo);

            memoryRequirements = m_device.getBufferMemoryRequirements(stagingBuffer);

            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = p_device->GetMemoryType(memoryRequirements.memoryTypeBits,
                                                                         vk::MemoryPropertyFlagBits::eHostVisible |
                                                                         vk::MemoryPropertyFlagBits::eHostCoherent);

            vk::DeviceMemory stagingMemory = m_device.allocateMemory(memoryAllocateInfo);

            m_device.bindBufferMemory(stagingBuffer, stagingMemory, 0);
            void *data;
            m_device.mapMemory(stagingMemory, 0, memoryRequirements.size, static_cast<vk::MemoryMapFlagBits>(0), &data);
            memcpy(data, texture.data(), texture.size());
            m_device.unmapMemory(stagingMemory);

            std::vector<vk::BufferImageCopy> bufferCopyRegions;
            uint32_t offset = 0;

            for (uint32_t i = 0; i < m_mipLevels; ++i)
            {
                vk::BufferImageCopy bufferCopyRegion;
                bufferCopyRegion.imageSubresource.aspectMask     = vk::ImageAspectFlagBits::eColor;
                bufferCopyRegion.imageSubresource.mipLevel       = i;
                bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
                bufferCopyRegion.imageSubresource.layerCount     = 1;
                bufferCopyRegion.imageExtent.width               = static_cast<uint32_t>(texture[i].extent().x);
                bufferCopyRegion.imageExtent.height              = static_cast<uint32_t>(texture[i].extent().y);
                bufferCopyRegion.imageExtent.depth               = 1;
                bufferCopyRegion.bufferOffset                    = offset;

                bufferCopyRegions.push_back(bufferCopyRegion);

                offset += static_cast<uint32_t>(texture[i].size());
            }

            // Create optimal tiled target image
            vk::ImageCreateInfo imageCreateInfo;
            imageCreateInfo.imageType     = vk::ImageType::e2D;
            imageCreateInfo.format        = p_format;
            imageCreateInfo.mipLevels     = m_mipLevels;
            imageCreateInfo.arrayLayers   = 1;
            imageCreateInfo.samples       = vk::SampleCountFlagBits::e1;
            imageCreateInfo.tiling        = vk::ImageTiling::eOptimal;
            imageCreateInfo.sharingMode   = vk::SharingMode::eExclusive;
            imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;
            imageCreateInfo.extent.width  = m_width;
            imageCreateInfo.extent.height = m_height;
            imageCreateInfo.extent.depth  = 1;
            imageCreateInfo.usage         = p_imageUsageFlags;
            if (!(imageCreateInfo.usage & vk::ImageUsageFlagBits::eTransferDst))
            {
                imageCreateInfo.usage |= vk::ImageUsageFlagBits::eTransferDst;
            }

            m_image = m_device.createImage(imageCreateInfo);

            memoryRequirements = m_device.getImageMemoryRequirements(m_image);

            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = p_device->GetMemoryType(memoryRequirements.memoryTypeBits,
                                                                         vk::MemoryPropertyFlagBits::eDeviceLocal);

            m_deviceMemory = m_device.allocateMemory(memoryAllocateInfo);
            m_device.bindImageMemory(m_image, m_deviceMemory, 0);

            vk::ImageSubresourceRange subresourceRange;
            subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            subresourceRange.baseMipLevel = 0;
            subresourceRange.levelCount = m_mipLevels;
            subresourceRange.layerCount = 1;

            VulkanHelpers::SetImageLayout(
                    copyCommandBuffer,
                    m_image,
                    vk::ImageLayout::eUndefined,
                    vk::ImageLayout::eTransferDstOptimal,
                    subresourceRange);

            copyCommandBuffer.copyBufferToImage(stagingBuffer,
                                                m_image,
                                                vk::ImageLayout::eTransferDstOptimal,
                                                static_cast<uint32_t >(bufferCopyRegions.size()),
                                                bufferCopyRegions.data());

            m_imageLayout = p_imageLayout;

            VulkanHelpers::SetImageLayout(
                    copyCommandBuffer,
                    m_image,
                    vk::ImageLayout::eTransferDstOptimal,
                    p_imageLayout,
                    subresourceRange);

            p_device->FlushCommandBuffer(copyCommandBuffer, p_copyQueue);

            m_device.freeMemory(stagingMemory);
            m_device.destroyBuffer(stagingBuffer);
        }
        else
        {
            assert(formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);

            vk::ImageCreateInfo imageCreateInfo;
            imageCreateInfo.imageType = vk::ImageType::e2D;
            imageCreateInfo.format = p_format;
            imageCreateInfo.extent.width = m_width;
            imageCreateInfo.extent.height = m_height;
            imageCreateInfo.extent.depth = 1;
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = vk::SampleCountFlagBits::e1;
            imageCreateInfo.tiling = vk::ImageTiling::eLinear;
            imageCreateInfo.usage = p_imageUsageFlags;
            imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;
            imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;

            vk::Image mappableImage = m_device.createImage(imageCreateInfo);
            memoryRequirements = m_device.getImageMemoryRequirements(mappableImage);

            memoryAllocateInfo = memoryRequirements.size;

            memoryAllocateInfo.memoryTypeIndex = p_device->GetMemoryType(memoryRequirements.memoryTypeBits,
                                                                         vk::MemoryPropertyFlagBits::eHostVisible |
                                                                         vk::MemoryPropertyFlagBits::eHostCoherent);

            vk::DeviceMemory mappableMemory = m_device.allocateMemory(memoryAllocateInfo);
            m_device.bindImageMemory(mappableImage, mappableMemory, 0);

            vk::ImageSubresource subResource;
            subResource.aspectMask = vk::ImageAspectFlagBits::eColor;
            subResource.mipLevel = 0;

            vk::SubresourceLayout subResourceLayout = m_device.getImageSubresourceLayout(mappableImage, subResource);
            void *data;
            m_device.mapMemory(mappableMemory, 0, memoryRequirements.size, static_cast<vk::MemoryMapFlagBits>(0),
                               &data);
            memcpy(data, texture[subResource.mipLevel].data(), texture[subResource.mipLevel].size());
            m_device.unmapMemory(mappableMemory);

            m_image = mappableImage;
            m_deviceMemory = mappableMemory;
            m_imageLayout = p_imageLayout; //was incorrect in the original!

            VulkanHelpers::SetImageLayout(copyCommandBuffer, m_image, vk::ImageAspectFlagBits::eColor,
                                          vk::ImageLayout::eUndefined, m_imageLayout);

            p_device->FlushCommandBuffer(copyCommandBuffer, p_copyQueue);
        }

        vk::SamplerCreateInfo samplerCreateInfo;
        samplerCreateInfo.magFilter = vk::Filter::eLinear;
        samplerCreateInfo.minFilter = vk::Filter::eLinear;
        samplerCreateInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
        samplerCreateInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
        samplerCreateInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
        samplerCreateInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.compareOp = vk::CompareOp::eNever;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = (useStaging) ? static_cast<float>(m_mipLevels) : 0.0f;
        samplerCreateInfo.maxAnisotropy = p_device->m_enabledFeatures.samplerAnisotropy
                                          ? p_device->m_deviceProperties.limits.maxSamplerAnisotropy : 1.0f;
        samplerCreateInfo.anisotropyEnable = VK_TRUE;
        samplerCreateInfo.borderColor = vk::BorderColor::eFloatOpaqueWhite;

        m_imageSampler = m_device.createSampler(samplerCreateInfo);

        vk::ImageViewCreateInfo viewCreateInfo;
        viewCreateInfo.viewType = vk::ImageViewType::e2D;
        viewCreateInfo.format = p_format;
        viewCreateInfo.components = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB,
                                     vk::ComponentSwizzle::eA};
        viewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        viewCreateInfo.subresourceRange.levelCount = (useStaging) ? m_mipLevels : 1;
        viewCreateInfo.image = m_image;

        m_imageView = m_device.createImageView(viewCreateInfo);

        UpdateDescriptor();
    }
};

struct VulkanCubemap : VulkanBaseTexture
{
    void LoadFromFile(
            VulkanDevice* p_device,
            std::string p_filename,
            vk::Format p_format,
            vk::Queue p_copyQueue)
    {
        gli::texture_cube cubeTexture(gli::load(p_filename));

        if(cubeTexture.empty())
        {
            std::stringstream error;
            error << "Could not load " << p_filename;
            throw std::runtime_error(error.str());
        }

        m_width     = static_cast<uint32_t>(cubeTexture.extent().x);
        m_height    = static_cast<uint32_t>(cubeTexture.extent().y);
        m_mipLevels = static_cast<uint32_t>(cubeTexture.levels());

        WrappedVulkanBuffer stagingbuffer;
        p_device->CreateBuffer(vk::BufferUsageFlagBits::eTransferSrc,
                               vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                               &stagingbuffer,
                               cubeTexture.size(),
                               cubeTexture.data());

        vk::ImageCreateInfo imageCreateInfo;
        imageCreateInfo.imageType       = vk::ImageType::e2D;
        imageCreateInfo.format          = p_format;
        imageCreateInfo.mipLevels       = m_mipLevels;
        imageCreateInfo.samples         = vk::SampleCountFlagBits::e1;
        imageCreateInfo.tiling          = vk::ImageTiling ::eOptimal;
        imageCreateInfo.usage           = vk::ImageUsageFlagBits::eSampled;
        imageCreateInfo.sharingMode     = vk::SharingMode::eExclusive;
        imageCreateInfo.initialLayout   = vk::ImageLayout::eUndefined;
        imageCreateInfo.extent          = vk::Extent3D(m_width, m_height, 1);
        imageCreateInfo.usage           = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
        imageCreateInfo.arrayLayers     = 6;
        imageCreateInfo.flags           = vk::ImageCreateFlagBits ::eCubeCompatible;

        m_image = p_device->m_logicalDevice.createImage(imageCreateInfo);

        vk::MemoryRequirements memoryRequirements = p_device->m_logicalDevice.getImageMemoryRequirements(m_image);

        vk::MemoryAllocateInfo memoryAllocateInfo;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = p_device->GetMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        m_deviceMemory = p_device->m_logicalDevice.allocateMemory(memoryAllocateInfo);

        p_device->m_logicalDevice.bindImageMemory(m_image, m_deviceMemory, 0);

        //Starts the copy command immediately
        vk::CommandBuffer copyCommand = p_device->CreateCommandBuffer(vk::CommandBufferLevel::ePrimary, true);

        std::vector<vk::BufferImageCopy> bufferCopyRegions;
        uint32_t offset = 0;

        for (uint32_t face = 0; face < 6; ++face)
        {
            for (uint32_t level = 0; level < m_mipLevels; ++level)
            {
                vk::Extent3D imageExtent;
                imageExtent.width  = static_cast<uint32_t>(cubeTexture[face][level].extent().x);
                imageExtent.height = static_cast<uint32_t>(cubeTexture[face][level].extent().y);
                imageExtent.depth  = 1;

                vk::ImageSubresourceLayers subresource;
                subresource.aspectMask     = vk::ImageAspectFlagBits ::eColor;
                subresource.mipLevel       = level;
                subresource.baseArrayLayer = face;
                subresource.layerCount     = 1;

                vk::BufferImageCopy bufferCopyRegion;
                bufferCopyRegion.imageExtent      = imageExtent;
                bufferCopyRegion.imageSubresource = subresource;
                bufferCopyRegion.bufferOffset     = offset;

                bufferCopyRegions.push_back(bufferCopyRegion);

                offset += cubeTexture[face][level].size();
            }
        }

        vk::ImageSubresourceRange subresourceRange;
        subresourceRange.aspectMask   = vk::ImageAspectFlagBits::eColor;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount   = m_mipLevels;
        subresourceRange.layerCount   = 6;

        VulkanHelpers::SetImageLayout(copyCommand,
                                      m_image,
                                      vk::ImageLayout::eUndefined,
                                      vk::ImageLayout::eTransferDstOptimal,
                                      subresourceRange);

        copyCommand.copyBufferToImage(stagingbuffer.m_buffer,
                                      m_image,
                                      vk::ImageLayout::eTransferDstOptimal,
                                      static_cast<uint32_t>(bufferCopyRegions.size()),
                                      bufferCopyRegions.data());

        m_imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        VulkanHelpers::SetImageLayout(copyCommand,
                                      m_image,
                                      vk::ImageLayout::eTransferDstOptimal,
                                      m_imageLayout,
                                      subresourceRange);

        p_device->FlushCommandBuffer(copyCommand, p_copyQueue, true);

        vk::SamplerCreateInfo samplerCreateInfo;
        samplerCreateInfo.magFilter = vk::Filter::eLinear;
        samplerCreateInfo.minFilter = vk::Filter::eLinear;
        samplerCreateInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
        samplerCreateInfo.addressModeU = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.addressModeV = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.addressModeW = vk::SamplerAddressMode::eClampToEdge;
        samplerCreateInfo.mipLodBias = 0.0f;
        samplerCreateInfo.compareOp = vk::CompareOp::eNever;
        samplerCreateInfo.minLod = 0.0f;
        samplerCreateInfo.maxLod = m_mipLevels;
        samplerCreateInfo.borderColor = vk::BorderColor ::eFloatOpaqueWhite;
        samplerCreateInfo.maxAnisotropy = 1.0f;

        if (p_device->m_deviceFeatures.samplerAnisotropy)
        {
            samplerCreateInfo.maxAnisotropy = p_device->m_deviceProperties.limits.maxSamplerAnisotropy;
            samplerCreateInfo.anisotropyEnable = vk::Bool32(true);
        }

        m_imageSampler = p_device->m_logicalDevice.createSampler(samplerCreateInfo);

        vk::ImageViewCreateInfo viewCreateInfo;
        viewCreateInfo.viewType = vk::ImageViewType ::eCube;
        viewCreateInfo.format = p_format;
        viewCreateInfo.components = {vk::ComponentSwizzle ::eR, vk::ComponentSwizzle ::eG, vk::ComponentSwizzle ::eB, vk::ComponentSwizzle ::eA};
        viewCreateInfo.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };
        viewCreateInfo.subresourceRange.layerCount = 6;
        viewCreateInfo.subresourceRange.levelCount = m_mipLevels;
        viewCreateInfo.image = m_image;

        m_imageView = p_device->m_logicalDevice.createImageView(viewCreateInfo);

        stagingbuffer.Destroy();
    }
};


#endif //VULKANENGINE_VULKANTEXTURE_HPP
