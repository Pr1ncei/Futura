/*
    This application has no purpose yet :P

    Author: Prince Pamintuan
    Date: December 07, 2025 (10:21PM)

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

    // @TODO: SEGMENTATION FAULT BUG
    // BUKAS::Log::enableReportGlErrors();

    if (!window.Initialize())
    {
        return -1; 
    }

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