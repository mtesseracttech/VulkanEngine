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

#include "Texture.hpp"
#include "Logger.hpp"

struct MeshVertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh
{
public:
    bool LoadFromFile(const std::string &p_filePath);

    const std::vector<MeshVertex>& GetVertices();
    const std::vector<unsigned int>& GetIndices();

private:
    std::vector<MeshVertex> m_vertices;
    std::vector<unsigned int> m_indices;

    bool InitializeFromAssimpScene(const aiScene *p_scene, const std::string &p_filePath);

    void InitializeMesh(unsigned int p_index, const aiMesh *p_mesh);
};


#endif //VULKANENGINE_MESH_HPP
