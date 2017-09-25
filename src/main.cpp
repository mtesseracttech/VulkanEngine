#include <iostream>
#include "VulkanDrawer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main()
{
    VulkanDrawer app;

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