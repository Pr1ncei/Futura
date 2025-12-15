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
     *      Last Modified on:    December 15, 2025 (8:42AM)
     */

    #include <iostream>
    #include <vector>
    #include "glad/glad.h"
    #include <GLFW/glfw3.h>
    #include "core/c_window.h"
    #include "core/c_input.h"
    #include "graphics/g_renderer.h"
    #include "../assets/models/Container.h"
    #include "../assets/models/Plane.h"
    #include "graphics/g_shader.h"
    #include "graphics/g_texture.h"
    #include "utils/u_log.h"
    
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float deltaTime = 0.0f; 
    float lastFrame = 0.0f; 

    int main(int, char**)
    {   
        Window window;
        Input input; 
        if (!window.Initialize(&camera, &input))
        {
            return -1; 
        }
        
        glEnable(GL_DEPTH_TEST);

        // Enable OpenGL debugging/logging, uncomment this if you want to disable OpenGL's debugging feature
        BUKAS::Log::enableReportGlErrors();


        Shader containerShader("../assets/shaders/Container.vs", "../assets/shaders/Container.fs");
        Renderer renderer(Models::vertices, 5);
 
        Renderer planeRenderer(Models::planeVertices, Models::planeVertexCount, 5);
 
        /*
            TESTING PURPOSES ONLY, I HAVEN'T IMPLEMENTED A MANAGER FOR THIS -<-
            I'll be testing if the camera and texture still works 
        */

        
        Texture texture; 
        texture.Add(
            Texture::Dimensions::D2,
            Texture::Wrap::Repeat,
            Texture::Mipmap::LinearLinear,
            true, 
            "../assets/texture/gate.jpg"
        );
        
        Texture groundTexture; 
        groundTexture.Add(
            Texture::Dimensions::D2,
            Texture::Wrap::Repeat, 
            Texture::Mipmap::LinearLinear,
            true, 
            "../assets/texture/grass.jpg"
        );

        containerShader.Use();
        containerShader.setInt("texture", 0);

        std::vector<GLuint> textures = {
            texture.GetID()
        };

        std::vector<GLuint> groundTextures = { groundTexture.GetID() };

        while (!window.ShouldClose())
        { 
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;


            input.Process(window.getWindow(), camera, deltaTime);
            glClearColor(0.58f, 0.925f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            
            texture.Use(textures);
            containerShader.Use();

            // TODO: Add a manager to handle this instead putting this on the main file
   
            glm::mat4 projection = glm::perspective(
                glm::radians(camera.Zoom),
                800.0f / 600.0f,
                0.1f, 
                100.0f
            );
            containerShader.setMat4("projection", projection);

            glm::mat4 view = camera.GetViewMatrix();
            containerShader.setMat4("view", view);

            for (unsigned int i = 0; i < 2; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, Models::cubePositions[i]);
                float angle = static_cast<float>(glfwGetTime()) * 25.0f * i; 
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                containerShader.setMat4("model", model);
                renderer.Draw();
            }
            groundTexture.Use(groundTextures);
            glm::mat4 groundModel = glm::mat4(1.0f); // The Identity Matrix
            containerShader.setMat4("model", groundModel);
            planeRenderer.Draw();

            window.Update();
        }
        renderer.Dispose();
        planeRenderer.Dispose();
        window.Terminate();

        return 0; 
    }