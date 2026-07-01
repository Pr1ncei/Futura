#include "pch.h"
#include "r_Material.h"

namespace FuturaLibrary
{
	Material::Material(const Ref<Shader>& shader, const Ref<Texture2D>& albedoTexture)
		: m_Shader(shader), m_AlbedoTexture(albedoTexture)
	{
	}

	void Material::Bind() const
	{
		FT_CORE_ASSERT(m_Shader, "Material has no shader!");

		m_Shader->Bind();
		if (m_AlbedoTexture)
		{
			m_AlbedoTexture->Bind(0);
			m_Shader->SetInt("u_Texture", 0);
		}
	}
}
