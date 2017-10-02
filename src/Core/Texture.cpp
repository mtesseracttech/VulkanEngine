//
// Created by MTesseracT on 2-10-2017.
//

#include "Texture.hpp"

Texture::Texture(const aiTexture * p_texture)
{
    m_size = glm::ivec2(p_texture->mWidth, p_texture->mHeight);
    m_texels = p_texture->pcData;
}