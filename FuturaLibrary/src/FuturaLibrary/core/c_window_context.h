/**
 *  @file c_window_context.h
 *
 *  @brief Defines the lightweight non-owning context shared with GLFW callbacks.
 *
 *  The input system uses this structure through GLFW's window user pointer to
 *  access the active input handler and camera without making either object
 *  globally owned by GLFW code.
 *
 *      @author:             Prince Pamintuan
 *      @date:               January 03, 2026
 *      Last Modified on:    July 01, 2026
 */

#pragma once

class Input;
class Camera;

struct WindowContext
{
	Input* input = nullptr;
	Camera* camera = nullptr;
};
