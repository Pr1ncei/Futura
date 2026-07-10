#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/graphics/g_texture.h"

namespace FuturaLibrary
{
	enum class MaterialTextureType
	{
		Albedo,
		Normal,
		Lightmap,
		Specular,
		Custom
	};

	struct MaterialTexture
	{
		std::string UniformName;
		Ref<Texture2D> Texture;
		uint32_t Slot = 0;
		MaterialTextureType Type = MaterialTextureType::Custom;
	};

	class FT_API Material
	{
	public:
		Material() = default;
		explicit Material(const Ref<Shader>& shader);
		Material(const Ref<Shader>& shader, const Ref<Texture2D>& albedoTexture);

		void Bind() const;

		void SetShader(const Ref<Shader>& shader);
		void SetTexture(const std::string& uniformName, const Ref<Texture2D>& texture, uint32_t slot, MaterialTextureType type = MaterialTextureType::Custom);
		void SetAlbedoTexture(const Ref<Texture2D>& texture, uint32_t slot = 0);
		void SetLightmapTexture(const Ref<Texture2D>& texture, uint32_t slot = 1);

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat3(const std::string& name, const glm::mat3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		const Ref<Shader>& GetShader() const { return m_Shader; }
		const std::vector<MaterialTexture>& GetTextures() const { return m_Textures; }

	private:
		MaterialTexture* FindTexture(const std::string& uniformName);
		const MaterialTexture* FindTexture(const std::string& uniformName) const;
		void BindTextures() const;
		void ApplyUniforms() const;

		Ref<Shader> m_Shader;
		std::vector<MaterialTexture> m_Textures;
		std::unordered_map<std::string, int> m_IntUniforms;
		std::unordered_map<std::string, float> m_FloatUniforms;
		std::unordered_map<std::string, glm::vec2> m_Float2Uniforms;
		std::unordered_map<std::string, glm::vec3> m_Float3Uniforms;
		std::unordered_map<std::string, glm::vec4> m_Float4Uniforms;
		std::unordered_map<std::string, glm::mat3> m_Mat3Uniforms;
		std::unordered_map<std::string, glm::mat4> m_Mat4Uniforms;
	};
}
