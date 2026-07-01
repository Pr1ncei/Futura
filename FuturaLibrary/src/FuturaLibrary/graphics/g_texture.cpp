/**
 *  @file g_texture.cpp
 * 
 *  @brief Implements the Texture class used for loading, initializing,
 *        configuring, and binding OpenGL textures.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 10, 2025 (1:51PM)
 *      Last Modified on:       January 03, 2026 (1:48PM)
 */

#include "pch.h"
#include "g_texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace FuturaLibrary
{
	Texture2D::Texture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
	{
		FT_PROFILE_FUNCTION;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(const std::string& path)
	{
		FT_PROFILE_FUNCTION;

		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		FT_CORE_ASSERT(data, "Failed to load texture!");

		m_Width = static_cast<uint32_t>(width);
		m_Height = static_cast<uint32_t>(height);

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else
		{
			stbi_image_free(data);
			FT_CORE_ASSERT(false, "Unsupported texture channel count!");
			return;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(
			m_RendererID,
			0,
			0,
			0,
			m_Width,
			m_Height,
			m_DataFormat,
			GL_UNSIGNED_BYTE,
			data
		);
		glGenerateTextureMipmap(m_RendererID);

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		FT_PROFILE_FUNCTION;
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		FT_PROFILE_FUNCTION;
		FT_CORE_ASSERT(m_RendererID != 0, "Attempted to bind an uninitialized Texture2D");
		glBindTextureUnit(slot, m_RendererID);
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		return CreateRef<Texture2D>(width, height);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		return CreateRef<Texture2D>(path);
	}
}
