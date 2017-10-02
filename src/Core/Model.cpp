//
// Created by MTesseracT on 2-10-2017.
//

#include <vulkan/vulkan.h>
#include "Model.hpp"

bool Model::LoadFromFile(const std::string &p_filePath)
{
    Logger::Log ("Attempting to load " +  p_filePath);

    bool success = false;
    Assimp::Importer importer;

    const aiScene *pScene = importer.ReadFile(p_filePath.c_str(),
                                              aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                              aiProcess_JoinIdenticalVertices);

    if (pScene)
    {
        success = InitializeFromAssimpScene(pScene, p_filePath);
    }
    else
    {
        Logger::Log("Error parsing" + p_filePath + ":" + importer.GetErrorString(), LogError);
    }

    return success;
}

bool Model::InitializeFromAssimpScene(const aiScene *p_scene, const std::string &p_filePath)
{
    try
    {
        m_meshes.resize(p_scene->mNumMeshes);

        for (int i = 0; i < p_scene->mNumMeshes; ++i)
        {
            const aiMesh * assimpMesh = p_scene->mMeshes[i];
            m_meshes[i] = new Mesh(assimpMesh);
        }

        m_textures.resize(p_scene->mNumTextures);

        for (int j = 0; j < p_scene->mNumTextures; ++j)
        {
            const aiTexture * assimpTexture = p_scene->mTextures[j];
            m_textures[j] = new Texture(assimpTexture);
        }

        return true;
    }
    catch (const std::runtime_error & err)
    {
        Logger::Log(std::string(err.what()), LogError);
        return false;
    }
}

const std::vector<Mesh *> &Model::GetMeshes()
{
    return m_meshes;
}

const std::vector<Texture *> &Model::GetTextures()
{
    return m_textures;
}
