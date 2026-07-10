/**
 *  @file r_ImGuiRenderer.cpp
 *
 *  @brief Implements the Dear ImGui GLFW/OpenGL renderer bridge.
 *
 *      @author:             Prince Pamintuan
 *      @date:               July 10, 2026
 *      Last Modified on:    July 10, 2026
 */

#include "pch.h"
#include "r_ImGuiRenderer.h"

#include "FuturaLibrary/core/c_window.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace FuturaLibrary
{
	namespace
	{
		bool s_Initialized = false;
	}

	void ImGuiRenderer::Initialize(Window& window)
	{
		FT_CORE_ASSERT(!s_Initialized, "ImGuiRenderer is already initialized!");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
		ImGui_ImplOpenGL3_Init("#version 450");

		s_Initialized = true;
	}

	void ImGuiRenderer::Shutdown()
	{
		if (!s_Initialized)
			return;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		s_Initialized = false;
	}

	void ImGuiRenderer::BeginFrame()
	{
		FT_CORE_ASSERT(s_Initialized, "ImGuiRenderer has not been initialized!");

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::EndFrame()
	{
		FT_CORE_ASSERT(s_Initialized, "ImGuiRenderer has not been initialized!");

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImGuiRenderer::IsInitialized()
	{
		return s_Initialized;
	}
}
