#include "pch.h"
#include "g_Shader.h"
#include "FuturaLibrary/renderer/r_Renderer.h"
#include "g_VertexArray.h"
#include "FuturaLibrary/utils/u_FileIO.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace FuturaLibrary
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")				return GL_VERTEX_SHADER;
		if (type == "fragment")				return GL_FRAGMENT_SHADER;
		if (type == "compute")				return GL_COMPUTE_SHADER;
		if (type == "tessellationControl")	return GL_TESS_CONTROL_SHADER;
		if (type == "tessellationEval")		return GL_TESS_EVALUATION_SHADER;
		if (type == "geometry")				return GL_GEOMETRY_SHADER;

		FT_CORE_ASSERT(false, "Shader Type is unknown or does not exist");
		return 0;
	}

	Shader::Shader(const std::string& filePath, const std::string& globalIncludeSource) : m_filePath(filePath)
	{
		FT_PROFILE_FUNCTION;
		std::string source = FileIO::ReadFile(filePath);
		source = std::regex_replace(source, std::regex("#includeGlobalSource"),
			globalIncludeSource);
		FT_CORE_ASSERT(!source.empty(), "Empty shader source read");
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
		Compile(shaderSources);
		m_Name = FileIO::ExtractNameFromFilePath(filePath);
	}

	Shader::~Shader()
	{
		FT_PROFILE_FUNCTION;
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		FT_PROFILE_FUNCTION;
		glUseProgram(m_RendererID);
	}

	/*
		Setting Uniforms
	*/
	void Shader::SetInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Int Location Not Found!");
		glProgramUniform1i(m_RendererID, location, value);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Float Location Not Found!");
		glProgramUniform1f(m_RendererID, location, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Float 2 Location Not Found!");
		glProgramUniform2f(m_RendererID, location, values.x, values.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Float 3 Location Not Found!");
		glProgramUniform3f(m_RendererID, location, values.x, values.y, values.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Float 4 Location Not Found!");
		glProgramUniform4f(m_RendererID, location, values.x, values.y, values.z, values.w);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Matrix 3 Location Not Found!");
		glProgramUniformMatrix3fv(m_RendererID, location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		FT_CORE_ASSERT(location != -1, "Matrix 4 Location Not Found!");
		glProgramUniformMatrix4fv(m_RendererID, location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	Ref<Shader> Shader::Create(const std::string& filePath, const std::string& globalIncludeSource)
	{
		return std::make_shared<Shader>(filePath, globalIncludeSource);
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		FT_PROFILE_FUNCTION;
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t endLine = source.find_first_of("\r\n", pos);
			FT_CORE_ASSERT(endLine != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, endLine - begin);

			FT_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", endLine);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(
				nextLinePos,
				(pos == std::string::npos ? source.size() : pos) - nextLinePos
			);
		}
		return shaderSources;
	}

	// This function takes multiple GLSL shader source strings, compiles them, 
	// links them into a single OpenGL shader program, and 
	// stores the resulting program ID.
	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		FT_PROFILE_FUNCTION;

		GLuint program = glCreateProgram();
		FT_CORE_ASSERT(shaderSources.size() <= 5, "We don't support that many shaders");

		std::array<GLenum, 5> glShaderIDs{};
		int glShaderIDIndex = 0;

		// Compile Each Shader
		for (auto&& [key, value] : shaderSources)
		{
			// Create the Shader Object 
			GLenum type = key;
			const std::string& source = value;
			GLuint shader = glCreateShader(type);

			// Upload GLSL src
			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			{
				// Compile Shader
				FT_PROFILE_SCOPE("Compile shader source code");
				glCompileShader(shader);
			}
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				FT_CORE_ERROR("{0} -> {1}", m_filePath, infoLog.data());
				FT_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		{
			FT_PROFILE_SCOPE("Link shader program");
			glLinkProgram(program);
		}

		{
			FT_PROFILE_SCOPE("Check for shader linking errors");
			GLint isLinked = 0;
			{
				FT_PROFILE_SCOPE("Get link status");
				glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			}
			if (isLinked == GL_FALSE) {
				GLint maxLength = 0;
				{
					FT_PROFILE_SCOPE("Get info log length");
					glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
				}

				std::vector<GLchar> infoLog(maxLength);
				{
					FT_PROFILE_SCOPE("Get program info");
					glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
				}

				{
					FT_PROFILE_SCOPE("Delete the program");
					glDeleteProgram(program);
				}

				for (auto id : glShaderIDs) {
					glDeleteShader(id);
				}

				FT_CORE_ERROR("{0}", infoLog.data());
				FT_CORE_ASSERT(false, "Shader link failure!");
				return;
			}
		}

		for (int i = 0; i < glShaderIDIndex; i++) {
			{
				FT_PROFILE_SCOPE("Detach shader");
				GLenum id = glShaderIDs[i];
				if (id == 0)
					continue;
				glDetachShader(program, id);
			}
		}
		m_RendererID = program;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName(); 
		Add(name, shader); 
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		FT_CORE_ASSERT(!Exists(name), "Shader already exists");
		m_Shaders[name] = shader; 
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& source)
	{
		auto shader = Shader::Create(source, m_GlobalIncludeSource + m_GlobalIncludeFileSource);
		Add(shader); 
		return shader; 
	}

	void ShaderLibrary::SetGlobalIncludeSource(const std::string& source)
	{
		m_GlobalIncludeSource = source;
	}

	void ShaderLibrary::SetGlobalIncludeFile(const std::string& filepath)
	{
		m_GlobalIncludeFileSource = FileIO::ReadFile(filepath);
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		FT_CORE_ASSERT(Exists(name), "Shader doesn't exists");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}