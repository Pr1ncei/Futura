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

#include <string>
#include <queue>
#include <FuturaLibrary/Core/c_core.h>
#include "g_TextureSubImage2D.h"

namespace FuturaLibrary
{
	class Texture
	{
	public: 
		virtual ~Texture() = default; 
		virtual uint32_t GetWidth() const = 0; 
		virtual uint32_t GetHeight() const = 0; 
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public: 
		Texture2D(uint32_t width, uint32_t heihgt); 
	};
}

#endif