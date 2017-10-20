//
// Created by MTesseracT on 20-10-2017.
//

#include "GameBase.hpp"

int main()
{
    GameBase game;
    try
    {
        game.Run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void GameBase::Run()
{
    Logger::Log("Game Starting!");
    m_renderer = new VulkanRendererBase();
    m_renderer->Initialize();
}
