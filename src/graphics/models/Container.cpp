#include "Container.h"

namespace Models
{
    std::vector<GLfloat> vertices = 
    {
      // POSITIONS          COLORS                TEXTURE COORDS
      0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,      
      0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f, 
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
      -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,     0.0f, 1.0f
    };

    std::vector<GLuint> indices = 
    {
        0, 1, 3,
        1, 2, 3
    };
}