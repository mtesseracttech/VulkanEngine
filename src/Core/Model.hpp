//
// Created by MTesseracT on 2-10-2017.
//

#ifndef VULKANENGINE_MODEL_HPP
#define VULKANENGINE_MODEL_HPP

#include "Core/Mesh.hpp"
#include "Texture.hpp"

#include <vector>

class Mesh;

class Model
{
public:
    bool LoadFromFile(const std::string &p_filePath);
    const std::vector<Mesh*>& GetMeshes();
    const std::vector<Texture*>& GetTextures();
private:
    std::vector<Mesh*> m_meshes;
    std::vector<Texture*> m_textures;

    bool InitializeFromAssimpScene(const aiScene *p_scene, const std::string &p_filePath);
};


#endif //VULKANENGINE_MODEL_HPP
