/**
 *  @file log.cpp
 * 
 *  Implements the OpenGL utilities declared in log.h
 * 
 *  This file defines the callback that processes OpenGL debug messages
 *  the function that enables OpenGL's debug reporting features
 * 
 *      @author:                Prince Pamintuan
 *      @date:                  December 08, 2025 (6:02PM)
 *      Last Modified on:       December 15, 2025 (8:43PM)
 */

#include "pch.h"
#include "u_log.h"

    void GLAPIENTRY BUKAS::Log::glDebugOutput(GLenum source, 
                                       GLenum type, 
                                       unsigned int id, 
                                       GLenum severity, 
                                       GLsizei length, 
                                       const char* message, 
                                       const void* userParam)
    {
        /**  
         * Filter noise and irrelevant debug messages.
         * 
         *      ID:         Description:
         *      131169 -    Buffer usage info (driver notification)        
         *      131185 -    Redundant state change
         *      131218 -    Shader/program recompiled by driver
         *      131204 -    General performance warning or driver notifcation
        */
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
        if (type == GL_DEBUG_TYPE_PERFORMANCE) return;

        std::cout << "--------------------\n";
        std::cout << "Debug message (" << id << "): " << message << '\n';

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:               std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:       std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:       std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:             std::cout << "Source: Other"; break;
        } std::cout << '\n';

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined   Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << '\n';

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:            std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:          std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:             std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    std::cout << "Severity: notification";
        } std::cout << '\n';
        std::cout << '\n';
    }

    void BUKAS::Log::enableReportGlErrors()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

