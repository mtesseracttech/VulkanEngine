//
// Created by mtesseract on 10-10-17.
//

#ifndef VULKANENGINE_VULKANINTERFACE_HPP
#define VULKANENGINE_VULKANINTERFACE_HPP


#include "VulkanLayer.hpp"

struct VulkanImageInfo
{
    VkImage                         image;
    VkDeviceMemory                  imageMemory;
    VkSampler                       sampler;
    VkImageView                     imageView;
};

class VulkanInterface : public VulkanLayer
{
public:
    void Run() override;
    VulkanImageInfo LoadImage(const std::string & p_fileName);
protected:
    void Cleanup() override;

    void InitializeVulkan() override;
};


#endif //VULKANENGINE_VULKANINTERFACE_HPP
