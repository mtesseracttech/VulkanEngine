//
// Created by MTesseracT on 20-10-2017.
//

#include <Utility/GameTimer.hpp>
#include "GameBase.hpp"
#include "KeyInput.hpp"

int main()
{
    GameBase game;
    try
    {
        //Initialize base systems
        game.InitializeRenderer();
        game.InitializeGameTimer();
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
    //Sets up the base of the renderer
    m_renderer.Initialize();
    //Sets up the render type specific things
    m_renderer.Prepare();
}

void GameBase::InitializeInput()
{
    Logger::Log("Initializing Key Input Manager");
    KeyInput::Initialize(m_renderer.GetWindow()->GetGlfwWindow());
}

void GameBase::RunGame()
{
    //Getting the window wrapper
    WrappedVulkanWindow * window = m_renderer.GetWindow();

    //Basic game speed
    SetGameSpeed(60);

    //Lag compensation variables
    double previous = GameTimer::Current();
    double lag = 0.0;

    //Uncontrolled base loop, to test FPS, timed loop will be added in when render works well
    while(!window->ShouldClose())
    {
        //Let GLFW deal with its events
        glfwPollEvents();

        //Update the delta component of
        GameTimer::Update();

        //Calculating how many cycles the update needs to run
        double current = GameTimer::Current();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        //Running the game update (multiple times) if needed
        while (lag > m_timePerUpdate)
        {
            m_gameWorld.Update();
            lag -= m_timePerUpdate;
        }

        //Preparing to render
        m_renderer.UpdateUniformBuffers();
        //Render Frame
        m_renderer.RenderFrame();
        //Wait till GPU is done
        m_renderer.DeviceWaitIdle(); //Could likely be integrated into the renderframe function
    }

    m_renderer.Cleanup();
}

void GameBase::InitializeGameTimer()
{
    GameTimer::Reset();
}

//How many game updates per second happen
void GameBase::SetGameSpeed(int p_tps)
{
    if(p_tps > 0)
    {
        m_ticksPerSecond = p_tps;
        m_timePerUpdate  = 1.0 / p_tps;
    }
}

