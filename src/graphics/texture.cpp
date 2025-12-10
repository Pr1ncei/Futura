#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include <iostream>

void Texture::Add(Dimensions dimension, 
                Wrap wrapper,
                Mipmap mipmap,
                bool flipped, 
                const char *TexturePath)
{   
    glGenTextures(1, &m_texture);
    glBindTexture((GLenum)dimension, m_texture);
    
    Texture::Initialize(dimension, wrapper, mipmap, flipped, TexturePath);
}

void Texture::Initialize(Dimensions dimension, 
                         Wrap wrapper, 
                         Mipmap mipmap,
                         bool flipped,
                         const char *TexturePath
                        )
{
    GLenum dim = (GLenum)dimension;

    // Texture Wrapping Parameters 
    glTexParameteri(dim, GL_TEXTURE_WRAP_S, (GLint)wrapper);
    glTexParameteri(dim, GL_TEXTURE_WRAP_T, (GLint)wrapper);

    // Texture Filtering Parameters
    glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, (GLenum)mipmap);
    glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load Image, Create Texture, and Generate Mipmaps 
    stbi_set_flip_vertically_on_load(flipped);

    data = stbi_load(TexturePath, &m_width, &m_height, &m_nrChannels, 0);

    if (!data) 
    {
        std::cout << "Failed to load texture: " << TexturePath << '\n';
        return;
    }

    GLenum format = (m_nrChannels == 1 ? GL_RED :
                     m_nrChannels == 3 ? GL_RGB :
                     GL_RGBA);

    glTexImage2D(dim, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(dim);

    Texture::Dispose();
}

void Texture::Dispose()
{ 
    stbi_image_free(data);
}

void Texture::Use(const std::vector<GLuint>& textures)
{
    for (size_t i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}