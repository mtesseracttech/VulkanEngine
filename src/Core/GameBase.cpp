//
// Created by MTesseracT on 20-10-2017.
//

#include "GameBase.hpp"
#include "KeyInput.hpp"

int main()
{
    GameBase game;
    try
    {
        //Initialize base systems
        game.InitializeRenderer();
        game.InitializeInput();

        //Runs the game
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
    //Sets up the base of the renderer
    m_renderer->Initialize();
    //Sets up the render type specific things
    m_renderer->Prepare();
}

void GameBase::InitializeInput()
{
    Logger::Log("Initializing Key Input Manager");
    KeyInput::Initialize(m_renderer->GetWindow()->GetWindow());
}

void GameBase::RunGame()
{
    //Getting the inner GLFW window reference
    GLFWwindow * window = m_renderer->GetWindow()->GetWindow();
    //Uncontrolled base loop, to test FPS, timed loop will be added in when render works well
    while(glfwWindowShouldClose(window) != GLFW_TRUE)
    {
        glfwPollEvents();
        m_renderer->UpdateUniformBuffers();
        m_renderer->RenderFrame();
        m_renderer->DeviceWaitIdle(); //Could likely be integrated into the renderframe function
    }

    delete m_renderer;
}