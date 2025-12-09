/**
 *  @file renderer.h
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
 *      Last Modified on:       December 09, 2025 (1:52PM)
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
class Renderer  
{
private:
    // Vertex Data (acts like a points) stored as floats
    std::vector<GLfloat> vertices;
    
    // Index Data used for indexed drawing instead of manually drawing with vertex data
    std::vector<GLuint> indices;
    
    // Number of vertices and indices
    size_t m_vertexCount, m_indexCount;
    
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
     * @brief Constructs a Renderer object and initializes vertex and index data.
     *
     * This constructor copies the provided vertex and index arrays into internal
     * vectors and sets up the counts for later buffer allocation.
     *
     * @param vertexData Pointer to the array of vertex data.
     * @param vertexCount Number of vertices in the vertexData array.
     * @param indexData Pointer to the array of index data.
     * @param indexCount Number of indices in the indexData array.
     */
    Renderer(GLfloat* vertexData, size_t vertexCount, GLuint* indexData, size_t indexCount);

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