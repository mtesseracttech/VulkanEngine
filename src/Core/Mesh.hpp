//
// Created by MTesseracT on 28-9-2017.
//

#ifndef VULKANENGINE_MESH_HPP
#define VULKANENGINE_MESH_HPP

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <string>
#include <vector>
#include <fstream>

#include "../Utility/Logger.hpp"

struct MeshVertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh
{
public:
    Mesh(const aiMesh *p_mesh);
    const std::vector<MeshVertex>& GetVertices();
    const std::vector<unsigned int>& GetIndices();

private:
    std::vector<MeshVertex>         m_vertices;
    std::vector<uint32_t>           m_indices;
};

#endif //VULKANENGINE_MESH_HPP