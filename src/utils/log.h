#pragma once 
#ifndef LOG_H
#define LOG_H
#include <glad/glad.h>

namespace BUKAS
{
    class Log
    {
    public:
        static void GLAPIENTRY glDebugOutput(
            GLenum source, 
            GLenum type, 
            unsigned int id,
            GLenum severity, 
            GLsizei length, 
            const char* message, 
            const void* userParam
        );

        static void enableReportGlErrors();
    };
}

#endif