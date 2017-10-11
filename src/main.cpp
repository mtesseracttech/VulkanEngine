#include <iostream>
#include "Core/Mesh.hpp"
#include "Core/Model.hpp"
#include "glm/ext.hpp"
#include "Utility/Constants.hpp"
#include "Core/VulkanLayer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Core/VulkanInterface.hpp>

int main()
{

    VulkanInterface vulkan;
    try
    {
        vulkan.Run();
        //vulkan.Run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}