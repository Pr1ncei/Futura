#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Renderer
{
private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    size_t m_vertexCount, m_indexCount;
    GLuint VAO, VBO, EBO;

public: 
    Renderer(GLfloat* vertexData, size_t vertexCount, GLuint* indexData, size_t indexCount);

    void BindBuffers();
    void LinkVertexAttr();
    void Dispose();
    void Draw();

};

#endif