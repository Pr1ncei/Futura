/**
 *  @file shader.cpp
 * 
 *  @brief Implements the Shader class for compiling, linking, and managing OpenGL shader programs.
 * 
 *  This file contains the constructor, which loads and compiles vertex and
 *  fragment shaders, links them into a program, and helper methods to activate
 *  the program and set uniform variables.
 *  
 *  Errors during compilation or linking are printed to the console. 
 * 
 *      @author:                  Prince Pamintuan
 *      @date:                    December 08, 2025 (6:50PM)
 *      Last Modified on:         December 09, 2025 (1:14PM)
 */

#include "shader.h"

/**
 *  @brief Loads, compiles, and links vertex and fragment shaders into a shader program.
 *  
 *  This constructor performs the following steps:
 *  1. Reads shader source files from disk using 'std::ifstream'
 *  2. Streams the file contents into memory using 'std::stringstream'
 *  3. Stores the source code in 'std::string' objects for compilation
 *  4. Compiles the vertex and fragment shaders 
 *  5. Links the shaders into a single shader program 'ID' and deletes individual shaders
 * 
 *      Classes Used:       Description:
 *          ifstream            Allows safe file reading with exception handling
 *          stringstream        Used to read the entire file contents into memory
 *          string              Holds shader source as a null-terminated C string 
 * 
 *  @param vertexPath - Path to the vertex shader source file
 *  @param fragmentPath - Path to the fragment shader source file
 */
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode; 
    std::string fragmentCode; 
    std::ifstream vShaderFile; 
    std::ifstream fShaderFile;

    // Enable exceptions on file streams
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Read shader files into strings
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR: Shader File not successfully read\n"; 
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment; 
    int success;
    char infoLog[512];

    // Compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // Link shaders into a program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const 
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}