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
     *      Last Modified on:    December 13, 2025 (10:26PM)
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

        Renderer renderer(Models::vertices, 5);
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

        containerShader.setInt("texture", 0);

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            800.0f / 600.0f,
            0.0f,
            100.0f
        );
        containerShader.setMat4("projection", projection);

        std::vector<GLuint> textures = {
            texture.GetID()
        };

        while (!window.ShouldClose())
        { 
            input.Process(window.getWindow());
            glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture.Use(textures);
            containerShader.Use();


            // TODO: Add a manager to handle this instead putting this on the main file
            glm::mat4 view = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(0.0f, 0.0f, -3.0f)
            );
            containerShader.setMat4("view", view);

            float angle = static_cast<float>(glfwGetTime());

            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, Models::cubePositions[i]);
                model = glm::rotate(model, angle + i, glm::vec3(1.0f, 0.3f, 0.5f));

                containerShader.setMat4("model", model);
                renderer.Draw();
            }


            window.Update();
        }
        renderer.Dispose();
        window.Terminate();

        return 0; 
    }