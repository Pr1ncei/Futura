#include "pch.h"
#include "r_Material.h"

namespace FuturaLibrary
{
	Material::Material(const Ref<Shader>& shader)
		: m_Shader(shader)
	{
	}

	Material::Material(const Ref<Shader>& shader, const Ref<Texture2D>& albedoTexture)
		: Material(shader)
	{
		SetAlbedoTexture(albedoTexture);
	}

	void Material::Bind() const
	{
		FT_CORE_ASSERT(m_Shader, "Material has no shader!");

		m_Shader->Bind();
		BindTextures();
		ApplyUniforms();
	}

	void Material::SetShader(const Ref<Shader>& shader)
	{
		m_Shader = shader;
	}

	void Material::SetTexture(
		const std::string& uniformName,
		const Ref<Texture2D>& texture,
		uint32_t slot,
		MaterialTextureType type
	)
	{
		FT_CORE_ASSERT(!uniformName.empty(), "Material texture uniform name cannot be empty!");

		MaterialTexture* existingTexture = FindTexture(uniformName);
		if (existingTexture)
		{
			existingTexture->Texture = texture;
			existingTexture->Slot = slot;
			existingTexture->Type = type;
			return;
		}

		m_Textures.push_back({ uniformName, texture, slot, type });
	}

	void Material::SetAlbedoTexture(const Ref<Texture2D>& texture, uint32_t slot)
	{
		SetTexture("u_Texture", texture, slot, MaterialTextureType::Albedo);
	}

	void Material::SetLightmapTexture(const Ref<Texture2D>& texture, uint32_t slot)
	{
		SetTexture("u_LightmapTexture", texture, slot, MaterialTextureType::Lightmap);
	}

	void Material::SetInt(const std::string& name, int value)
	{
		m_IntUniforms[name] = value;
	}

	void Material::SetFloat(const std::string& name, float value)
	{
		m_FloatUniforms[name] = value;
	}

	void Material::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		m_Float2Uniforms[name] = value;
	}

	void Material::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		m_Float3Uniforms[name] = value;
	}

	void Material::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		m_Float4Uniforms[name] = value;
	}

	void Material::SetMat3(const std::string& name, const glm::mat3& value)
	{
		m_Mat3Uniforms[name] = value;
	}

	void Material::SetMat4(const std::string& name, const glm::mat4& value)
	{
		m_Mat4Uniforms[name] = value;
	}

	MaterialTexture* Material::FindTexture(const std::string& uniformName)
	{
		for (MaterialTexture& texture : m_Textures)
		{
			if (texture.UniformName == uniformName)
				return &texture;
		}

		return nullptr;
	}

	const MaterialTexture* Material::FindTexture(const std::string& uniformName) const
	{
		for (const MaterialTexture& texture : m_Textures)
		{
			if (texture.UniformName == uniformName)
				return &texture;
		}

		return nullptr;
	}

	void Material::BindTextures() const
	{
		for (const MaterialTexture& texture : m_Textures)
		{
			if (!texture.Texture)
				continue;

			texture.Texture->Bind(texture.Slot);
			m_Shader->SetInt(texture.UniformName, static_cast<int>(texture.Slot));
		}
	}

	void Material::ApplyUniforms() const
	{
		for (const auto& [name, value] : m_IntUniforms)
			m_Shader->SetInt(name, value);
		for (const auto& [name, value] : m_FloatUniforms)
			m_Shader->SetFloat(name, value);
		for (const auto& [name, value] : m_Float2Uniforms)
			m_Shader->SetFloat2(name, value);
		for (const auto& [name, value] : m_Float3Uniforms)
			m_Shader->SetFloat3(name, value);
		for (const auto& [name, value] : m_Float4Uniforms)
			m_Shader->SetFloat4(name, value);
		for (const auto& [name, value] : m_Mat3Uniforms)
			m_Shader->SetMat3(name, value);
		for (const auto& [name, value] : m_Mat4Uniforms)
			m_Shader->SetMat4(name, value);
	}
}
