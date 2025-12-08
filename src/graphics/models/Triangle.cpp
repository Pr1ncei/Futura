/**
 * Draws a triangle on a screen. 
 * This is used to test the rendering system 
 * for this applcation.
 * 
 * Author: Prince Pamintuan
 * Date: December 09, 2025 (1:00AM)
 */
#include "Triangle.h"

namespace Models
{
    std::vector<GLfloat> vertices = 
    {
      // POSITIONS          COLORS                  
      0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,          
      0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,        
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,       
      -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,       
    };

    std::vector<GLuint> indices = 
    {
        0, 1, 2
    };
}