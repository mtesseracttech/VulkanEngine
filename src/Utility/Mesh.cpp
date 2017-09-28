//
// Created by MTesseracT on 28-9-2017.
//

#include "Mesh.hpp"

bool Mesh::LoadFromFile(const std::string &p_filePath)
{
    std::cout << "Attempting to load " << p_filePath << std::endl;

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


bool Mesh::InitializeFromAssimpScene(const aiScene *p_scene, const std::string &p_filePath)
{
    //Yeah, this isn't correct, going to have to create submesh logic
    std::cout << p_scene->mNumMeshes << std::endl;
    m_vertices.resize(p_scene->mNumMeshes);

    for (unsigned int i = 0; i < m_vertices.size(); i++)
    {
        const aiMesh *assimpMesh = p_scene->mMeshes[i];
        InitializeMesh(i, assimpMesh);
    }
}

void Mesh::InitializeMesh(unsigned int p_index, const aiMesh *p_mesh)
{
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D zero(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
    {
        const aiVector3D *pos = &(p_mesh->mVertices[i]);
        const aiVector3D *normal = p_mesh->HasNormals() ? &(p_mesh->mNormals[i]) : &zero;
        const aiVector3D *texCoord = p_mesh->HasTextureCoords(0) ? &(p_mesh->mTextureCoords[0][i]) : &zero;

        MeshVertex v{{pos->x, pos->y, pos->z},
                     {normal->x, normal->y, normal->z},
                     {texCoord->x, texCoord->y}};

        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
    {
        const aiFace &face = p_mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    m_indices = indices;
    m_vertices = vertices;
}

const std::vector<MeshVertex> &Mesh::GetVertices()
{
    return m_vertices;
}

const std::vector<unsigned int> &Mesh::GetIndices()
{
    return m_indices;
}

