/**
 *  @file texture.h
 * 
 *  @brief Implements the Texture class used for loading, initializing,
 *        configuring, and binding OpenGL textures.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 10, 2025 (1:51PM)
 *      Last Modified on:       December 15, 2025 (8:43PM)
 */

#include "pch.h"
#include "g_texture.h"


/**
 * @brief Creates and configures a new OpenGL texture.
 *
 * This function generates and binds a texture ID, then delegates the
 * remaining setup to Texture::Initialize().
 *
 * @param dimension   Specifies the texture target (e.g., GL_TEXTURE_2D).
 * @param wrapper     The wrapping mode (e.g., GL_REPEAT, GL_CLAMP_TO_EDGE).
 * @param mipmap      Filtering mode for minification, including mipmaps.
 * @param flipped     If true, the loaded image will be flipped vertically.
 * @param TexturePath Path to the image to be loaded.
 */
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

/**
 * @brief Loads an image file and initializes texture parameters.
 *
 * This function configures texture wrapping, filtering, loads image pixel
 * data using stb_image, determines the correct color format based on channel
 * count, uploads the texture to OpenGL, and generates mipmaps if required.
 *
 * @param dimension   Texture target (GL_TEXTURE_2D, GL_TEXTURE_3D, etc.).
 * @param wrapper     Texture wrapping mode.
 * @param mipmap      Minification filter with or without mipmaps.
 * @param flipped     Whether to flip the image vertically before loading.
 * @param TexturePath File system path to the texture image.
 */
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

/**
 * @brief Frees image data loaded by stb_image.
 *
 * This function must be called after uploading pixel data to the GPU
 * to avoid memory leaks.
 */
void Texture::Dispose()
{ 
    stbi_image_free(data);
}

/**
 * @brief Binds a list of textures to sequential texture units.
 *
 * Useful when rendering shaders that use multiple textures.
 *
 * Example:
 *   textures[0] → GL_TEXTURE0  
 *   textures[1] → GL_TEXTURE1  
 *   ...
 *
 * @param textures A vector of OpenGL texture IDs to activate and bind.
 */
void Texture::Use(const std::vector<GLuint>& textures)
{
    for (size_t i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    
}