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
        m_props.r            = p_color.r;
        m_props.g            = p_color.g;
        m_props.b            = p_color.b;
        m_props.roughness    = p_roughness;
        m_props.metallicness = p_metallicness;
    }

    //Data to be pushed to gpu
    struct Properties{
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float roughness    = 0.0f;
        float metallicness = 0.0f;
    } m_props;
};

#endif //VULKANENGINE_SIMPLEPBRMATERIAL_HPP
