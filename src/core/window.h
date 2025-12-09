/**
 *  @file window.h
 * 
 *  @brief Defines the Window class, which encapsulates GLFW window creation,
 *        context management, and basic event handling for OpenGL applications.
 * 
 *  This class handles initializing GLFW, creating a window, managing the OpenGL context,
 *  handling window resize events, and providing a simple interface for checking whether
 *  the window should close and updating the frame buffer.
 * 
 *      @author:                  Prince Pamintuan
 *      @date:                    December 08, 2025 (4:47PM)
 *      Last Modified on:         December 09, 2025 (2:13PM)
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @brief Encapsulates a GLFW window and provides methods for window management.
 *
 * The Window class abstracts the GLFWwindow pointer and provides functionality for
 * initialization, frame updates, and termination. It also sets a framebuffer size
 * callback to handle window resizing.
 */
class Window
{
private: 
    // Pointer to the GLFW window object
    GLFWwindow* m_window;

    /**
     * @brief Callback function called by GLFW when the window is resized.
     *
     * Adjusts the OpenGL viewport to match the new window size.
     *
     * @param window Pointer to the GLFW window.
     * @param width  New width of the window.
     * @param height New height of the window.
     */
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    /**
     * @brief Returns the underlying GLFW window pointer.
     * @return Pointer to GLFWwindow.
     */
    GLFWwindow* getWindow() const { return m_window; }
    
    /**
     * @brief Initializes GLFW, creates the window, and loads OpenGL function pointers via GLAD.
     *
     * This function must be called before any OpenGL rendering or context usage.
     *
     * @return True if initialization was successful, false otherwise.
     */
    bool Initialize();

    /**
     * @brief Checks whether the window should close.
     *
     * Typically used in the render loop to determine whether to exit.
     *
     * @return True if the window should close, false otherwise.
     */ 
    bool ShouldClose();

    /**
     * @brief Updates the window by polling events and swapping buffers.
     *
     * Must be called each frame to process input events and display the rendered frame.
     */
    void Update();  

    /**
     * @brief Terminates GLFW and cleans up resources associated with the window.
     */
    void Terminate();
};

#endif