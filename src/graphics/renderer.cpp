/**
 *  @file renderer.cpp
 * 
 *  @brief Implements the Renderer class responsible for managing vertex and index buffers,
 *        setting up vertex array objects (VAO), and drawing models using OpenGL.
 * 
 *  The Renderer class encapsulates the creation and management of OpenGL buffer objects
 *  and provides a simple interface for binding, linking vertex attributes, drawing, and
 *  disposing resources.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 09, 2025 (12:05AM)
 *      Last Modified on:       December 09, 2025 (2:11PM)
 */

#include "renderer.h"
#include <iostream>

/**
 * Constructs a Renderer Object
 * 
 * Copies vertex and index data into internal storage and initializes
 * OpenGL buffers and the vertex array object
 * 
 * @param vertexData - Pointer to the array of vertex floats
 * @param vertexCount - Number of floats in the vertex array
 * @param indexData - Pointer to the array of indices
 * @param indexCount - Number of indices in the index array
 */
Renderer::Renderer(GLfloat* vertexData, size_t vertexCount, GLuint* indexData, size_t indexCount)
{
    m_vertexCount = vertexCount;
    m_indexCount = indexCount;

    vertices.assign(vertexData, vertexData + vertexCount);
    indices.assign(indexData, indexData + indexCount);

    VAO = VBO = EBO = 0; 

    BindBuffers();
}

/**
 * Creates and binds VAO, VBO, and EBO.
 * 
 * Uploads vertex and index data to the GPU and configures 
 * vertex attributes.
 */
void Renderer::BindBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    Renderer::LinkVertexAttr();

    // glBindVertexArray(0);
}

/**
 * Configures vertex attribute pointers for the VAO
 * 
 * Adjust stride and offsets to match the layout of the vertex data. 
 * The current function assumes 8 floats per vertex:
 *      position                = 3
 *      colors                  = 3
 *      texture coordinates     = 2     
 */
void Renderer::LinkVertexAttr()
{
    glBindVertexArray(VAO);

    // Vertex Array Object
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Buffer Object
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Element Buffer Object
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Deletes OpenGL buffers and releases GPU resources.
 */
void Renderer::Dispose()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

/**
 * Draws the mesh using the currently bound VAO and EBO
 * 
 * Issues a glDrawElements call with the number of indices
 * stored in the renderer
 */
void Renderer::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}