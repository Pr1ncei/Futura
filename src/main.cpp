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
     *  @note When developing, do not add 'using namespace ___', show the WHOLE NAMESPACE PLEASE TO AVOID CONFUSION.
     *      @example
     *      Texture::Dispose() // CORRECT
     *      Dispose();         // INCORRECT + Creates Confusion
     *  
     *      @author:             Prince Pamintuan
     *      @date:               December 07, 2025 (10:21PM)
     *      Last Modified on:    December 10, 2025 (2:26PM)
     */

    #include <iostream>
    #include <vector>
    #include "glad/glad.h"
    #include <GLFW/glfw3.h>
    #include "core/window.h"
    #include "core/input.h"
    #include "graphics/renderer.h"
    #include "graphics/models/Container.h"
    #include "graphics/shader.h"
    #include "graphics/texture.h"
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

        Shader containerShader("../src/graphics/shaders/Container.vs", "../src/graphics/shaders/Container.fs");

        Renderer renderer(
            Models::vertices.data(), Models::vertices.size(),
            Models::indices.data(), Models::indices.size()
        );

        /*
            TESTING PURPOSES ONLY, I HAVEN'T IMPLEMENTED A MANAGER FOR THIS -<-
            I'll be testing if the texture feature works
        */

        Texture texture; 
        texture.Add(
            Texture::Dimensions::D2,
            Texture::Wrap::Repeat,
            Texture::Mipmap::LinearLinear,
            true, 
            "../src/graphics/texture/gate.jpg"
        );
        
        containerShader.Use();
        glUniform1i(glGetUniformLocation(containerShader.ID, "texture"), 0);

        containerShader.setInt("texture", 0);

        std::vector<GLuint> textures = {
            texture.GetID()
        };

        while (!window.ShouldClose())
        { 
            input.Process(window.getWindow());
            glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            texture.Use(textures);
            
            containerShader.Use();
            renderer.Draw();

            window.Update();
        }
        renderer.Dispose();
        window.Terminate();

        return 0; 
    }