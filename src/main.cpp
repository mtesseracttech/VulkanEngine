#include <iostream>
#include "VulkanTriangle.hpp"
#include "Utility/Logger.hpp"
#include <glfw/glfw3.h>

int main()
{

    /*
    VulkanEngine engine;


    try
    {
        engine.Run();
    }
    catch(const std::runtime_error& e)
    {
        Logger::Log(e.what(), LogType::LogError);
        return EXIT_FAILURE;
    }
    */

    VulkanTriangle app;

    try
    {
        app.Run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}