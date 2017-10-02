//
// Created by MTesseracT on 2-10-2017.
//

#ifndef VULKANENGINE_MODEL_HPP
#define VULKANENGINE_MODEL_HPP


#include "Mesh.hpp"
#include "Texture.hpp"

class Model
{
public:
    bool LoadFromFile(const std::string &p_filePath);
    const std::vector<Mesh*>& GetMeshes();
    const std::vector<Texture*>& GetTextures();
private:
    std::vector<Mesh*> m_meshes;
    std::vector<Texture*> m_textures;
    bool InitializeFromAssimpScene(const aiScene *pScene, const std::string &basic_string);
};


#endif //VULKANENGINE_MODEL_HPP
