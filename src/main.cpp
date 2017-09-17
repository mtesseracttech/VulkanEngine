#include <iostream>
#include "VulkanTriangle.hpp"

int main()
{
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