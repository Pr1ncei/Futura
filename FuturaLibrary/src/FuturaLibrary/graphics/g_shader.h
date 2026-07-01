/**
 *  @file g_shader.h
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 08, 2025 (6:50PM)
 *      Last Modified on:       December 30, 2025 (5:05PM)
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
typedef unsigned int GLenum;
namespace FuturaLibrary
{
    class FT_API Shader
    {
    public:
        Shader(const std::string& filePath, const std::string& globalIncludeSource); 
        ~Shader(); 

        void Bind() const; 
        const std::string& GetName() const { return m_Name; }

        // Setting Uniforms 
        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value); 
        void SetFloat2(const std::string& name, const glm::vec2& values);
        void SetFloat3(const std::string& name, const glm::vec3& values);
        void SetFloat4(const std::string& name, const glm::vec4& values);
        void SetMat3(const std::string& name, const glm::mat3& matrix); 
        void SetMat4(const std::string& name, const glm::mat4& matrix); 

        static Ref<Shader> Create(const std::string& filePath, const std::string& globalIncludeSource); 
        
    protected: 
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source); 
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources); 

        uint32_t m_RendererID; 
        std::string m_Name; 
        std::string m_filePath; 
    };

    class FT_API ShaderLibrary
    {
    public: 
        void Add(const Ref<Shader>& shader); 
        void Add(const std::string& name, const Ref<Shader>& shader); 
        Ref<Shader> Load(const std::string& source); 
        void SetGlobalIncludeSource(const std::string& source);
        void SetGlobalIncludeFile(const std::string& filePath); 

        Ref<Shader> Get(const std::string& name); 
        bool Exists(const std::string& name) const; 

    private: 
        std::unordered_map<std::string, Ref<Shader>> m_Shaders; 
        std::string m_GlobalIncludeSource; 
        std::string m_GlobalIncludeFileSource;  
    };
}
