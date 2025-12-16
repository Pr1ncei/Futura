/**
 *  @file texture.h
 * 
 *  @brief Defines the Texture class used for loading, initializing,
 *        configuring, and binding OpenGL textures.
 *  
 *  The Texture class acts as a wrapper around OpenGL texture creation.
 *  It supports 2D/3D textures, wrapping modes, mipmap settings, and
 *  simplified usage through enums and helper functions.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 10, 2025 (1:51PM)
 *      Last Modified on:       December 15, 2025 (8:43PM)
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

class Texture
{
private:

    /**
     *  @brief OpenGL texture object ID.
     * 
     *  Assigned by `glGenTextures` and used to identify and bind the
     *  texture in subsequent OpenGL operations.
     */
    GLuint m_texture;


    /**
     *  @brief Dimensions of the loaded texture image.
     * 
     *  These values are filled by `stbi_load` and describe
     *  the width, height, and number of channels in the image file.
     */
    GLint m_width, m_height, m_nrChannels;

    /**
     *  @brief Raw pixel data loaded from file.
     * 
     *  Managed through stb_image. Freed after uploading to the GPU.
     */
    unsigned char *data;

public:

    /**
     *  @brief Returns the OpenGL texture ID.
     * 
     *  Useful for shader uniform assignments and manual texture binding.
     * 
     *  @return GLuint - The OpenGL texture handle.
     */
    GLuint GetID() const { return m_texture; }

    // this is more like a design choice as it was much safer to code with, same with other enums :P
    enum class Dimensions : GLenum
    {
        D2               = GL_TEXTURE_2D,
        D3               = GL_TEXTURE_3D 
    };

    enum class Wrap : GLenum
    {
        Repeat           = GL_REPEAT,
        MirroeredRepeat  = GL_MIRRORED_REPEAT,
        ClampEdge        = GL_CLAMP_TO_EDGE,
        ClampBorder      = GL_CLAMP_TO_BORDER
    };

    enum class Mipmap : GLenum
    {
        None             = 0,
        NeartestNearest  = GL_NEAREST_MIPMAP_NEAREST,
        NearestLinear    = GL_NEAREST_MIPMAP_LINEAR,
        LinearNearest    = GL_NEAREST_MIPMAP_LINEAR,
        LinearLinear     = GL_LINEAR_MIPMAP_LINEAR
    };

    /**
     *  @brief Loads, configures, and initializes a texture in one call.
     * 
     *  This function loads an image using stb_image, binds a new OpenGL 
     *  texture object, sets wrapping and filtering options, and optionally 
     *  generates mipmaps depending on the selected filtering mode.
     * 
     *  @param dimension - Texture dimensionality (2D or 3D).
     *  @param wrapper - Texture wrapping mode.
     *  @param mipmap - Mipmap filtering mode (None disables mipmaps).
     *  @param flipped - Whether to vertically flip the image on load.
     *  @param TexturePath - Path to the texture image file.
     */
    void Add(Dimensions dimension, 
            Wrap wrapper = Wrap::Repeat, 
            Mipmap mipmap = Mipmap::None,
            bool flipped = false,
            const char *TexturePath = nullptr);
    
    /**
     *  @brief Initializes a texture using previously loaded data.
     * 
     *  This method creates and configures the OpenGL texture object, 
     *  applies wrapping and mipmap options, uploads the pixel data, and 
     *  generates mipmaps if required.
     * 
     *  @param dimension - Texture dimensionality (2D or 3D).
     *  @param wrapper - Texture wrapping mode.
     *  @param mipmap - Mipmap filtering mode.
     *  @param flipped - Whether the image should be flipped vertically before upload.
     *  @param TexturePath - Path to the texture file that will be loaded.
     */
    void Initialize(Dimensions dimension, 
                    Wrap wrapper = Wrap::Repeat, 
                    Mipmap mipmap = Mipmap::None, 
                    bool flipped=false,
                    const char *TexturePath = nullptr);

    /**
     *  @brief Deletes the OpenGL texture and frees pixel data.
     * 
     *  This should be called when the texture is no longer needed
     *  to prevent GPU memory leaks.
     */
    void Dispose();

    /**
     *  @brief Binds multiple textures to active OpenGL texture units.
     * 
     *  Texture units are assigned sequentially (0,1,2,...).  
     *  This is useful for shaders that use multiple texture samplers.
     * 
     *  @param textures - A list of OpenGL texture IDs to bind.
     */
    static void Use(const std::vector<GLuint>& textures);

};


#endif