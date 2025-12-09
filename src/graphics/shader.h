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
 *      Last Modified on:       December 09, 2025 (1:14PM)
 */

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

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
};


#endif