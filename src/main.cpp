#include <iostream>
#include "Core/Mesh.hpp"
#include "Core/Model.hpp"
#include "glm/ext.hpp"
#include "Utility/Constants.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main()
{

    try
    {
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}