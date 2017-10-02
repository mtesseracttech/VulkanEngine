//
// Created by MTesseracT on 28-9-2017.
//

#include <vulkan/vulkan.h>
#include "Mesh.hpp"

Mesh::Mesh(const aiMesh *p_mesh)
{
    const aiVector3D zero(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
    {
        const aiVector3D *pos = &(p_mesh->mVertices[i]);
        const aiVector3D *normal = p_mesh->HasNormals() ? &(p_mesh->mNormals[i]) : &zero;
        const aiVector3D *texCoord = p_mesh->HasTextureCoords(0) ? &(p_mesh->mTextureCoords[0][i]) : &zero;

        MeshVertex v{{pos->x, pos->y, pos->z},
                     {normal->x, normal->y, normal->z},
                     {texCoord->x, texCoord->y}};

        m_vertices.push_back(v);
    }

    for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
    {
        const aiFace &face = p_mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        m_indices.push_back(face.mIndices[0]);
        m_indices.push_back(face.mIndices[1]);
        m_indices.push_back(face.mIndices[2]);
    }
}

const std::vector<MeshVertex> &Mesh::GetVertices()
{
    return m_vertices;
}

const std::vector<uint32_t> &Mesh::GetIndices()
{
    return m_indices;
}