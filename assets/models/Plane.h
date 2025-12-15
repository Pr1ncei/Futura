// graphics/models/Plane.h

#include <glm/glm.hpp>
#include <vector>

namespace Models {
    // Vertices for a simple 2x2 plane in the XZ plane
    // (X, Y, Z, U, V) where Y is 0 for a flat ground
    float planeVertices[] = {
        // positions         // texture coords
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f, // Top Right
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f, // Top Left
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f, // Bottom Left

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f, // Top Right
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f, // Bottom Left
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f,  // Bottom Right
    };
    // The vertex count is 6 (vertices) * 5 (components per vertex)
    const size_t planeVertexCount = 30; 
}