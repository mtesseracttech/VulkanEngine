//
// Created by MTesseracT on 12-2-2018.
//

#ifndef VULKANENGINE_VECTORHELPER_HPP
#define VULKANENGINE_VECTORHELPER_HPP


#include <glm/vec3.hpp>

class VectorHelper
{
    static bool IsEqual(glm::vec3 p_vecA, glm::vec3 p_vecB, float p_delta);

    struct Dirs
    {
        static constexpr auto RHT = glm::vec3(1.0f, 0.0f, 0.0f);
        static constexpr auto LFT = glm::vec3(-1.0f, 0.0f, 0.0f);
        static constexpr auto UP  = glm::vec3(0.0f, 1.0f, 0.0f);
        static constexpr auto DWN = glm::vec3(0.0f, -1.0f, 0.0f);
        static constexpr auto FWD = glm::vec3(0.0f, 0.0f, 1.0f);
        static constexpr auto BCK = glm::vec3(0.0f, 0.0f, -1.0f);
    };
};


#endif //VULKANENGINE_VECTORHELPER_HPP
