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
 *      Last Modified on:       January 03, 2026 (1:48PM)
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <FuturaLibrary/Core/c_core.h>

namespace FuturaLibrary
{
	class FT_API Texture
	{
	public: 
		virtual ~Texture() = default; 
		virtual uint32_t GetWidth() const = 0; 
		virtual uint32_t GetHeight() const = 0; 
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class FT_API Texture2D : public Texture
	{
	public: 
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const std::string& path);
		~Texture2D() override;

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetID() const { return m_RendererID; }

		void Bind(uint32_t slot = 0) const override;

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_InternalFormat = 0;
		uint32_t m_DataFormat = 0;
	};
}

#endif
