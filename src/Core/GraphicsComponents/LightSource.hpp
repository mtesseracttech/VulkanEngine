//
// Created by MTesseracT on 8-2-2018.
//

#ifndef VULKANENGINE_LIGHTSOURCE_HPP
#define VULKANENGINE_LIGHTSOURCE_HPP


#include <glm/vec3.hpp>

enum LightType {
    Point,
    Directional,
    Spot
};

class LightSource
{
    LightType m_type         = Point;
    glm::vec3 m_color        = glm::vec3(1.0f); //White by default
    glm::vec3 m_position     = glm::vec3(0.0f); //Center by default
    float     m_intensity    = 0.0f;            //Off by default
    glm::vec3 m_attentuation = glm::vec3(0.0f); //No attenuation by default
};


#endif //VULKANENGINE_LIGHTSOURCE_HPP
