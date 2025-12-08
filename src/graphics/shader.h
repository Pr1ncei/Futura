/*

    Author: Prince Pamintuan
    Date: December 08, 2025 (6:50PM)

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
    unsigned int ID;
    
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const; 
    void setFloat(const std::string &name, float value) const;
};


#endif