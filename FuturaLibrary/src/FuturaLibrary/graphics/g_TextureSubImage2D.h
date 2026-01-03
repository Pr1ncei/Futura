#ifndef TEXTURE_SUBIMAGE_2D_H
#define TEXTURE_SUBIMAGE_2D_H

#include "pch.h"
#include <FuturaLibrary/core/c_core.h>
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	class TextureSubImage2D
	{
	public: 
		TextureSubImage2D(const std::string& path);
		~TextureSubImage2D(); 

		const std::string& GetName() const				{ return m_Name; };
		const texture_data* GetData() const				{ return m_Data; };
		const uint32_t GetWidth() const					{ return m_Width; };
		const uint32_t GetId() const					{ return m_ID; };
		const uint32_t GetHeight() const				{ return m_Height; };
		const glm::vec2& GetTexCoords() const			{ return m_TexCoords; };
		const int GetXOffset()							{ return m_XOffset; }
		const int GetYOffset()							{ return m_YOffset; }
		uint32_t GetChannels()							{ return m_Channels; }
		
		void SetId(uint32_t id)							{ m_ID = id; }
		void SetTexCoords(glm::vec2 texCoords)			{ m_TexCoords = texCoords; }
		void SetOffset(int xOffset, int yOffset);

		void Combine(const Ref<TextureSubImage2D> other);
		void ToRGBA();
		void FreeData();

	private: 
		// Texture Data 
		uint32_t m_ID, m_Width, m_Height, m_Channels; 
		texture_data* m_Data; 
		std::string m_Name; 

		int m_XOffset = -1, m_YOffset = -1;

		glm::vec2 m_TexCoords; 

		bool m_StbiLoaded = true;
	};
}

#endif 
