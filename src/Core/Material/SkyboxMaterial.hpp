//
// Created by MTesseracT on 6-11-2017.
//

#ifndef VULKANENGINE_SKYBOXMATERIAL_HPP
#define VULKANENGINE_SKYBOXMATERIAL_HPP

#include <Utility/Constants.hpp>
#include "AbstractMaterial.hpp"

class SkyboxMaterial : public AbstractMaterial
{
private:
    const uint32_t VERTEX_BUFFER_BIND_ID = 0;
public:

    void UpdateUbo(const GameObject &p_gameObject) override
    {

    }

    void CreatePipeline(vk::Device p_logicalDevice) override
    {
    }
};

#endif //VULKANENGINE_SKYBOXMATERIAL_HPP
