//
// Created by MTesseracT on 30-10-2017.
//

#ifndef VULKANENGINE_GAMEWORLD_HPP
#define VULKANENGINE_GAMEWORLD_HPP

#include "GameObject.hpp"
#include "Camera.hpp"

class GameWorld : GameObject
{
private:
    Camera m_camera;
protected:

public:
    const Camera& GetPrimaryCamera() const {return m_camera;}
};

#endif //VULKANENGINE_GAMEWORLD_HPP
