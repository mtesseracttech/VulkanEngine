#include <iostream>
#include "Core/Mesh.hpp"
#include "Core/Model.hpp"
#include "glm/ext.hpp"
#include "Utility/Constants.hpp"
#include "Core/VulkanLayer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main()
{
    VulkanLayer vulkan;
    try
    {
        vulkan.run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    /*
    Model * model = new Model;
    if(!model->LoadFromFile(Constants::MODEL_PATH + "robot.blend"))
    {
        Logger::Log("Model didn't load properly", LogError);
    }
    else
    {
        std::cout << "Meshes " << model->GetMeshes().size() << std::endl;

        for(const auto& mesh : model->GetMeshes())
        {
            std::cout << "Mesh indices " << mesh->GetIndices().size() <<std::endl;
            std::cout << "Mesh vertices " << mesh->GetVertices().size() << std::endl;
        }

        std::cout << "Textures " << model->GetTextures().size() << std::endl;
    }
     */
}