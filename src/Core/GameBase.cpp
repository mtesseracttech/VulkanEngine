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
    m_renderer = new SimpleRenderer();
    m_renderer->Initialize();
    m_renderer->Prepare();
}

void GameBase::RunGame()
{
    GLFWwindow * window = m_renderer->GetWindow()->GetWindow();
    while(glfwWindowShouldClose(window) != GLFW_TRUE)
    {
        glfwPollEvents();
        m_renderer->UpdateUniformBuffers();
        m_renderer->RenderFrame();
        m_renderer->DeviceWaitIdle();
    }

    delete m_renderer;
}