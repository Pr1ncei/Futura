/**
 *  @file r_ImGuiRenderer.h
 *
 *  @brief Declares the Dear ImGui frame bridge for the GLFW/OpenGL renderer.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#pragma once

#include "FuturaLibrary/core/c_core.h"

namespace FuturaLibrary
{
	class Window;

	class FT_API ImGuiRenderer
	{
	public:
		static void Initialize(Window& window);
		static void Shutdown();
		static void BeginFrame();
		static void EndFrame();
		static bool IsInitialized();
	};
}
