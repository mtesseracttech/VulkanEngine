//
// Created by MTesseracT on 22-11-2017.
//

#ifndef VULKANENGINE_GAMEWORLD_HPP
#define VULKANENGINE_GAMEWORLD_HPP

#include "Core/GameObject.hpp"
#include "Core/Camera.hpp"

class GameWorld : public GameObject
{
public:
    const Camera& GetCamera();
    void SetupCamera();
protected:
    Camera * m_camera;
};



#endif //VULKANENGINE_GAMEWORLD_HPP
