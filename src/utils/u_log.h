/**
 *  @file log.h
 * 
 *  @brief This file defines the OpenGL debug logging utilities for the Futura Engine, 
 *  initially BUKAS. 
 * 
 *  This utilities enable OpenGL's built-in debugging features and route debug
 *  output to the console with formatted messages.
 * 
 *  The logging system helps diagnose performance warnings, shader errors,
 *  invalid OpenGL usage, and driver-related issues.
 *      
 *      @author:             Prince Pamintuan
 *      @date:               December 08, 2025 (6:02PM)
 *      Last Modified on:    December 15, 2025 (8:42PM)
 */
#ifndef LOG_H
#define LOG_H
#include <glad/glad.h>

namespace BUKAS
{
    class Log
    {
    public:
        /**
         * @brief Callback for handling OpenGL debug messages.
         * 
         * OpenGL calls this function whenever a debug event occurs. 
         * The function prints information such as message source, type, 
         * severity, and the message content in a readable format.
         * 
         * Messages that are low value like redudant notifications, or 
         * specific known IDs are filtered out to reduce console noise
         * 
         * @param source - The source of the debug message 
         * @param type - The type of debug event 
         * @param id - The ID of the specific debug message
         * @param severity - The severity level of the message
         * @param length - The message string length
         * @param message - The actual debug message from OpenGL
         * @param userParam - Optional user paramater, usually unused
         */
        static void GLAPIENTRY glDebugOutput(
            GLenum source, 
            GLenum type, 
            unsigned int id,
            GLenum severity, 
            GLsizei length, 
            const char* message, 
            const void* userParam
        );

        /**
         * @brief Enables OpenGL debug output and registers the debug callback
         * 
         * @note This function must be called *AFTER* creating a valid OpenGL context.
         * Enabling it before the initialization will cause a Segmentation Fault 
         * Error.           
         *              - Don't do the same mistake that I did :3
         * 
         * This function enables the synchronous debug output to ensure messages
         * are printed immediately and in order. The debug message filter is 
         * configured to allow all messages through
         * 
         * Example:
         * @code
         *  Window window; 
         *  window.Initalize();
         *  BUKAS::Log::enableReportGlErrors();
         * @code
         * 
         */
        static void enableReportGlErrors();
    };
}

#endif