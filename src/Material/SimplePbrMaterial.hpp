//
// Created by mtesseract on 20-12-17.
//

#ifndef VULKANENGINE_SIMPLEPBRMATERIAL_HPP
#define VULKANENGINE_SIMPLEPBRMATERIAL_HPP

#include "AbstractMaterial.hpp"

struct SimplePbrMaterial// : AbstractMaterial
{
    SimplePbrMaterial() = default;;

    SimplePbrMaterial(glm::vec3 p_color, float p_roughness, float p_metallicness)
    {
        m_props.color        = p_color;
        m_props.roughness    = p_roughness;
        m_props.metallicness = p_metallicness;
    }

    //Data to be pushed to gpu
    struct Properties{
        glm::vec3 color    = glm::vec3(0,0,0);
        float roughness    = 0.0f;
        float metallicness = 0.0f;
    } m_props;
};

#endif //VULKANENGINE_SIMPLEPBRMATERIAL_HPP
