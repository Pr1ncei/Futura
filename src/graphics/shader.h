/**
 *  @file shader.h
 * 
 *  @brief Defines the Shader class used for compiling, linking, and 
 *        managing OpenGL shader programs
 * 
 *  This class loads vertex and fragment shader source code from files,
 *  compiles them, links them into a shader program, and provides helper
 *  functions to set uniform variables. It encapsulates typical shader
 *  management tasks into a simple interface
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 08, 2025 (6:50PM)
 *      Last Modified on:       December 13, 2025 (10:00PM)
 */

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    /**
     *  @brief OpenGL shader program ID
     * 
     *  This value is assigned after successful compilation and linking
     *  of the vertex and fragment shader stages.
     */
    unsigned int ID;
    
    /**
     *  @brief Constructs a Shader program by loading, compiling, and linking shaders.
     * 
     *  The constructor reads shader source code from files, compiles the vertex
     *  and fragment shaders, and links them into an OpenGL shader program.
     * 
     *  @param vertexPath - Path to the vertex shader source file
     *  @param fragmentPath - Path to the fragment shader source file
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     *  @brief Activates (uses) the shader program.
     * 
     *  This function calls `glUseProgran(ID)` and makes the shader acts 
     *  for subsequent draw calls and uniform updates.
     */
    void Use();

    /**
     *  @brief Sets a boolean uniform in the shader program.
     * 
     *  @param name - Name of the uniform variable in the shader
     *  @param value - Boolean value to set
     */
    void setBool(const std::string &name, bool value) const;

    /**
     *  @brief Sets an integer uniform in the shader program.
     * 
     *  @param name - Name of the uniform variable in the shader
     *  @param value - Integer value to set
     */
    void setInt(const std::string &name, int value) const; 
    
    /**
     *  @brief Sets a float uniform in the shader program.
     * 
     *  @param name - Name of the uniform variable in the shader
     *  @param value - Floating-point value to set
     */
    void setFloat(const std::string &name, float value) const;




    void setVec2(const std::string &name, const glm::vec2 &value) const 
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }

     void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }



};


#endif