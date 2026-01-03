#include "pch.h"
#include "g_TextureSubImage2D.h"
#include <stb_image.h>
#include "FuturaLibrary/core/c_core.h"

namespace FuturaLibrary
{
	TextureSubImage2D::TextureSubImage2D(const std::string& path)
	{
		FT_PROFILE_FUNCTION; 
		int width, height, channels; 
		stbi_set_flip_vertically_on_load(1); 
		stbi_uc* data = nullptr;
		{
			FT_PROFILE_SCOPE(
				"stbi-load - OpenGLTexture2D::OpenGLTexture2D(const std::string&))"
			);
			data = stbi_load(path.c_str(), &width, &height, &channels, 0); 
		}

		FT_CORE_ASSERT(data, "Failed to load image! " + path);
		FT_CORE_ASSERT(channels >= 1 && channels <= 4, "Unsupported channel count!");

		m_Width = width; 
		m_Height = height; 
		m_Channels = channels; 
		m_Data = data; 
		m_Name = FileIO::ExtractNameFromFilePath(path); 
		m_StbiLoaded = true;
	}

	TextureSubImage2D::~TextureSubImage2D()
	{
		if (m_Data) FreeData();
	}

	void TextureSubImage2D::SetOffset(int xOffset, int yOffset)
	{
		m_XOffset = xOffset;
		m_YOffset = yOffset; 
	}

	// Combines 'other' texture as a sub-image with offset
	void TextureSubImage2D::Combine(const Ref<TextureSubImage2D> other)
	{
		if (!other->m_Data) return;

		// Ensure both textures are RGBA for safe blending
		if (m_Channels != 4) ToRGBA();
		if (other->m_Channels != 4) other->ToRGBA();

		for (int y = 0; y < other->m_Height; y++)
		{
			for (int x = 0; x < other->m_Width; x++)
			{
				// Calculate destination pixel with offset
				int dstX = x + other->m_XOffset;
				int dstY = y + other->m_YOffset;

				// Skip if out of bounds
				if (dstX < 0 || dstX >= (int)m_Width || dstY < 0 || dstY >= (int)m_Height)
					continue;

				int dstIdx = (dstY * m_Width + dstX) * m_Channels;
				int srcIdx = (y * other->m_Width + x) * other->m_Channels;

				// Skip fully transparent pixels
				if (other->m_Data[srcIdx + 3] == 0)
					continue;

				memcpy(&m_Data[dstIdx], &other->m_Data[srcIdx], m_Channels);
			}
		}
	}

	// Converts 1,2,3 channel textures to RGBA
	// Never call this multiple times on the same texture 
	// since you'll be allocating the same memory again if you do it 
	void TextureSubImage2D::ToRGBA()
	{
		if (m_Channels == 4) return;

		uint32_t newChannels = 4;
		stbi_uc* newData = new stbi_uc[m_Width * m_Height * newChannels];

		for (int i = 0; i < m_Width * m_Height; i++) {
			int src = i * m_Channels;
			int dst = i * 4;

			switch (m_Channels) {
			case 1: // grayscale
				newData[dst + 0] = m_Data[src + 0];
				newData[dst + 1] = m_Data[src + 0];
				newData[dst + 2] = m_Data[src + 0];
				newData[dst + 3] = 255;
				break;

			case 2: // grayscale + alpha
				newData[dst + 0] = m_Data[src + 0];
				newData[dst + 1] = m_Data[src + 0];
				newData[dst + 2] = m_Data[src + 0];
				newData[dst + 3] = m_Data[src + 1];
				break;

			case 3: // RGB
				memcpy(&newData[dst], &m_Data[src], 3);
				newData[dst + 3] = 255;
				break;
			}
		}

		FreeData();
		m_Data = newData;
		m_Channels = newChannels;
		m_StbiLoaded = false;
	}

	void TextureSubImage2D::FreeData()
	{
		if (!m_Data) return; 

		if (m_StbiLoaded) stbi_image_free(m_Data);
		else			  delete[] m_Data;

		m_Data = nullptr; 
	}
}