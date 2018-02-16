//
// Created by MTesseracT on 20-10-2017.
//

#ifndef VULKANENGINE_GAMEBASE_HPP
#define VULKANENGINE_GAMEBASE_HPP

#include <stdexcept>
#include <iostream>
#include <Utility/Logger.hpp>
#include <Core/Renderers/SimpleRenderer.hpp>
#include <Core/Renderers/PbrRenderer.hpp>
#include <Core/Renderers/ImprovedRenderer.hpp>
#include <Core/MtVulkanAPI/Device/Display.hpp>

class GameBase
{
private:
    Display *        m_display = Display::Get();
    //ImprovedRenderer m_renderer;
    //PbrRenderer     m_renderer;
    int             m_ticksPerSecond;
    double          m_timePerUpdate;
    Timer           m_lagTimer;

    void SetGameSpeed(int p_tps);
public:
    void RunGame();

    void InitializeRenderer();
    void InitializeInput();
    void InitializeTimers();
};


#endif //VULKANENGINE_GAMEBASE_HPP
