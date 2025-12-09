/**
 *  @file main.cpp
 * 
 *  @brief This entry point for Futura, initially Bukas, a Doom-like 3D OpenGL game.
 * 
 *  It is a doom-like game that I currently planning to make for my portfolio. 
 *  This is used to understand more about 3D Computer Graphics with OpenGL
 *      
 *  This file demonstrates the basic setup and rendering loop for a 3D OpenGL project.
 *  It initializes the window, input handling, OpenGL debugging, shaders, and the renderer,
 *  and contains the main game/render loop where frames are drawn and updated.
 * 
 *      @author:             Prince Pamintuan
 *      @date:               December 07, 2025 (10:21PM)
 *      Last Modified on:    December 09, 2025 (2:25PM)
 */

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "core/window.h"
#include "core/input.h"
#include "graphics/renderer.h"
#include "graphics/models/Triangle.h"
#include "graphics/shader.h"
#include "utils/log.h"

int main(int, char**)
{
    Window window;
    Input input; 

    if (!window.Initialize())
    {
        return -1; 
    }

    // Enable OpenGL debugging/logging, uncomment this if you want to disable OpenGL's debugging feature
    BUKAS::Log::enableReportGlErrors();

    Shader triangleShader("../src/graphics/shaders/Triangle.vs", "../src/graphics/shaders/Triangle.fs");

    Renderer renderer(
        Models::vertices.data(), Models::vertices.size(),
        Models::indices.data(), Models::indices.size()
    );

    triangleShader.Use();

    while (!window.ShouldClose())
    { 
        input.Process(window.getWindow());
        glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleShader.Use();
        renderer.Draw();

        window.Update();
    }
    renderer.Dispose();
    window.Terminate();

    return 0; 
}