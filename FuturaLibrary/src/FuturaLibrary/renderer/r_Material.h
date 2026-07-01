#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/graphics/g_texture.h"

namespace FuturaLibrary
{
	class FT_API Material
	{
	public:
		Material() = default;
		Material(const Ref<Shader>& shader, const Ref<Texture2D>& albedoTexture);

		void Bind() const;

		const Ref<Shader>& GetShader() const { return m_Shader; }
		const Ref<Texture2D>& GetAlbedoTexture() const { return m_AlbedoTexture; }

	private:
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_AlbedoTexture;
	};
}
