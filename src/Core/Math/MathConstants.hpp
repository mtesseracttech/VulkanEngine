//
// Created by MTesseracT on 12-2-2018.
//

#ifndef VULKANENGINE_MATHCONSTANTS_HPP
#define VULKANENGINE_MATHCONSTANTS_HPP


#include <glm/glm.hpp>

class MathConstants
{
public:
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


#endif //VULKANENGINE_MATHCONSTANTS_HPP
