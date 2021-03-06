//
// Created by MTesseracT on 20-10-2017.
//

#ifndef VULKANENGINE_GAMEBASE_HPP
#define VULKANENGINE_GAMEBASE_HPP

#include <stdexcept>
#include <iostream>
#include <Utility/Logger.hpp>
#include <Core/Renderers/SimpleRenderer.hpp>

class GameBase
{
private:
    VulkanRendererBase * m_renderer = nullptr;
public:
    void InitializeRenderer();
    void RunGame();
};


#endif //VULKANENGINE_GAMEBASE_HPP
