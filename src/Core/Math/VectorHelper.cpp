//
// Created by MTesseracT on 12-2-2018.
//


#include <cmath>
#include "VectorHelper.hpp"

bool VectorHelper::IsEqual(glm::vec3 p_vecA, glm::vec3 p_vecB, float p_delta)
{
    for (int i = 0; i < 3; ++i)
    {
        if (std::fabs(p_vecA[i] - p_vecB[i]) > p_delta) return false;
    }
    return true;
}
