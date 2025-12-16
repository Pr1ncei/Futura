/**
 *  @file g_mesh.cpp
 * 
 *  @brief Implements the Renderer class responsible for managing OpenGL
 *         vertex array objects (VAO), vertex buffers (VBO), optional index
 *         buffers (EBO), and issuing draw calls.
 * 
 *  The Renderer class encapsulates the creation and management of OpenGL buffer objects
 *  and provides a simple interface for binding, linking vertex attributes, drawing, and
 *  disposing resources.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 09, 2025 (12:05AM)
 *      Last Modified on:       December 15, 2025 (8:23PM)
 */

/**
 * NOTE: 
 *              This will refactor in the next commit, naming g_mesh.cpp, 
 *              I realized that the renderer should only contain of when it will render
 *              and NOT the handling the geometry side of the mesh.
 */

#include "g_mesh.h"
#include <iostream>

/**
 * Constructs a Renderer for indexed geometry.
 * 
 * Copies vertex and index data into internal CPU-side storage,
 * then creates and initializes the corresponding OpenGL buffers
 * (VAO, VBO, and EBO).
 * 
 * @param vertexData   Pointer to an array of vertex attribute data
 * @param vertexCount  Total number of floats in the vertex array
 * @param indexData    Pointer to an array of index data
 * @param indexCount   Number of indices in the index array
 * @param stride       Number of floats per vertex
 */
Mesh::Mesh(GLfloat* vertexData, size_t vertexCount, GLuint* indexData, size_t indexCount, GLsizei stride) : m_stride(stride), m_indexCount(indexCount)
{
    m_vertexCount = vertexCount;
    m_indexCount = indexCount;

    m_vertices.assign(vertexData, vertexData + vertexCount);
    m_indices.assign(indexData, indexData + indexCount);
    m_vertexCount = vertexCount / stride;

    VAO = VBO = EBO = 0; 

    BindBuffers();
}

Mesh::Mesh(GLfloat* vertexData, size_t vertexCount, GLsizei stride) 
    : m_stride(stride), 
      m_indexCount(0), // <-- Critical: Must initialize index count to 0
      m_vertexCount(vertexCount / stride) // Calculate vertex count
{
    // Copy data
    m_vertices.assign(vertexData, vertexData + vertexCount);
    // m_indices is an empty vector by default construction, which is fine

    VAO = VBO = EBO = 0; 
    BindBuffers();
}

/**
 * Constructs a Renderer for non-indexed geometry.
 * 
 * This constructor is intended for meshes that do not use an
 * element buffer object (EBO). Rendering will be performed using
 * glDrawArrays.
 * 
 * @param vertices  Vector containing interleaved vertex attribute data
 * @param stride    Number of floats per vertex
 */
Mesh::Mesh(const std::vector<GLfloat>& vertices, GLsizei stride) : m_vertices(vertices), m_stride(stride), m_indexCount(0)
{
    m_vertexCount = vertices.size() / stride;
    VAO = VBO = EBO = 0; 

    BindBuffers();
}

/**
 * Generates and binds the required OpenGL buffer objects.
 * 
 * Creates:
 *  - A Vertex Array Object (VAO)
 *  - A Vertex Buffer Object (VBO)
 *  - An optional Element Buffer Object (EBO) if index data exists
 * 
 * Vertex and index data are uploaded to the GPU, and vertex
 * attribute pointers are configured.
 */
void Mesh::BindBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

    if (!m_indices.empty())
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
    }

    Mesh::LinkVertexAttr();
    glBindVertexArray(0);
}

/**
 *  Configures vertex attribute pointers for the VAO
 *  
 *  The attribute layout is defined using the provided stride. 
 *  The current implementation assumes the following layout:
 *      layout(location = 0) -> vec3 position (x,y,z)
 *      layout(location = 1) -> vec2 texture coordinates (u,v)
 * 
 *  Total floats per vertex: 5  
 */
void Mesh::LinkVertexAttr()
{
    glBindVertexArray(VAO);

    // Vertex Array Object
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Buffer Object
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, m_stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // // Element Buffer Object
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Deletes OpenGL buffers and releases GPU resources.
 */
void Mesh::Dispose()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

/**
 * Issues a draw call for the mesh.
 * 
 * If index data is present, glDrawElements is used.
 * Otherwise, glDrawArrays is issued.
 * 
 * The appropriate draw method is selected automatically
 * based on the presence of an element buffer.
 */
void Mesh::Draw()
{
    glBindVertexArray(VAO);
    
    if (m_indexCount > 0)
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    else 
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    
    glBindVertexArray(0);
}