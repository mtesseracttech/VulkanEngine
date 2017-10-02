//
// Created by MTesseracT on 2-10-2017.
//

#ifndef VULKANENGINE_TEXTURE_HPP
#define VULKANENGINE_TEXTURE_HPP


#include <assimp/texture.h>
#include <glm/vec2.hpp>
#include <vulkan/vulkan.hpp>

typedef const aiTexel* Texels;

class Texture
{
public:
    Texture(const aiTexture * p_texture);
    const glm::ivec2 GetSize(){return m_size;}
    Texels GetTexels(){return m_texels;}
private:
    glm::ivec2 m_size;
    Texels m_texels;
};


#endif //VULKANENGINE_TEXTURE_HPP
