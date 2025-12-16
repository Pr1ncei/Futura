/**
 *  @file g_renderer.h
 * 
 *  @brief Defines the Renderer class responsible for managing vertex and index buffers,
 *        setting up vertex array objects (VAO), and drawing models using OpenGL.
 * 
 *  The Renderer class encapsulates the creation and management of OpenGL buffer objects
 *  and provides a simple interface for binding, linking vertex attributes, drawing, and
 *  disposing resources.
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 09, 2025 (12:05AM)
 *      Last Modified on:       December 15, 2025 (8:28PM)
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

/**
 *  @brief Handles the creation, binding, and rendering of vertex and index data in OpenGL
 * 
 *  This class stores vertex and index data internally, manages OpenGL buffers (VAO, VBO, EBO),
 *  and provides functions for drawing and cleanup.
 */
class Mesh 
{
private:
    // Vertex Data (acts like a points) stored as floats
    std::vector<GLfloat> m_vertices;
    
    // Index Data used for indexed drawing instead of manually drawing with vertex data
    std::vector<GLuint> m_indices;
    
    // Number of vertices and indices
    size_t m_vertexCount, m_indexCount;
    GLsizei m_stride;

    /**
     *  Vertex Array Object (VAO) - Container for Vertex Attribute State    
     *      1. Which VBOs are bound
     *      2. How vertex attributes are laid out (position, color, texture coordinates)
     *      3. Which EBO is used
     * 
     *  Vertex Buffer Object (VBO) - GPU buffer that stores vertex data 
     *      - Used for storing vertex data for faster access instead of individually
     *      sending each vertex data
     * 
     *  Element Buffer Object (EBO) - GPU buffer that stores indices, not raw vertex data
     *      - Tells which vertices to connect when drawing triangles, lines, etc
     *      - Avoids duplicate vertex data for memory management
     */
    GLuint VAO, VBO, EBO;

public: 
    /**
     * Constructs a Renderer for indexed geometry.
     * 
     * @param vertexData   Pointer to an array of vertex attribute data
     * @param vertexCount  Total number of floats in the vertex array
     * @param indexData    Pointer to an array of index data
     * @param indexCount   Number of indices in the index array
     * @param stride       Number of floats per vertex
     */
    Mesh(GLfloat* vertexData, size_t vertexCount, GLuint* indexData, size_t indexCount, GLsizei stride);

    Mesh(GLfloat* vertexData, size_t vertexCount, GLsizei stride);


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
    Mesh(const std::vector<GLfloat>& vertices, GLsizei stride);


    /**
     * @brief Binds the VAO, VBO, and EBO and uploads vertex/index data to the GPU.
     *
     * After calling this, the renderer is ready for linking vertex attributes.
     */
    void BindBuffers();

    /**
     * @brief Links vertex attributes to the shader program.
     *
     * This function configures the layout of the vertex data for OpenGL rendering,
     * specifying positions, normals, texture coordinates, or other attributes.
     */
    void LinkVertexAttr();


    /**
     * @brief Deletes OpenGL buffers and frees resources associated with this renderer.
     */
    void Dispose();

    /**
     * @brief Draws the object using the currently bound shader and VAO.
     *
     * Typically called inside the render loop after activating the shader program.
     */
    void Draw();

};

#endif