//
// Created by MTesseracT on 30-10-2017.
//

#ifndef VULKANENGINE_GAMEWORLD_HPP
#define VULKANENGINE_GAMEWORLD_HPP

#include "GameObject.hpp"
#include "Camera.hpp"

class GameWorld : public GameObject
{
private:
    Camera m_camera;
public:
    const Camera& GetCamera() const
    {
        return m_camera;
    }


};

#endif //VULKANENGINE_GAMEWORLD_HPP
