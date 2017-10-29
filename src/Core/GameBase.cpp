//
// Created by MTesseracT on 20-10-2017.
//

#include "GameBase.hpp"

int main()
{
    GameBase game;
    try
    {
        game.InitializeRenderer();
        game.RunGame();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void GameBase::InitializeRenderer()
{
    Logger::Log("Initializing the renderer");
    m_renderer = new VulkanRendererBase();
    m_renderer->Initialize();
}

void GameBase::RunGame()
{
    while (!glfwWindowShouldClose(m_renderer->GetWindow()))
    {
        glfwPollEvents();
        std::cout << "Waiting for stuff to happen" << std::endl;
        m_renderer->DrawFrame();
    }

    m_renderer->DeviceWaitIdle();
}

GameBase::~GameBase()
{
    m_renderer->Cleanup();
}
