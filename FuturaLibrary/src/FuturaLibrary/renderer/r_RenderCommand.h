/*
	Low-level OpenGL command layer that actually calls glEnable, glClear, glDrawElements, etc.
*/


#pragma once

#include "FuturaLibrary/core/c_core.h"
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	enum class RenderCullFace
	{
		Back,
		Front,
		FrontAndBack
	};

	struct RenderViewport
	{
		int X = 0;
		int Y = 0;
		int Width = 0;
		int Height = 0;
	};

	struct RenderClearState
	{
		glm::vec4 Color = glm::vec4(0.0f);
		bool ClearColor = true;
		bool ClearDepth = true;
	};

	struct RenderState
	{
		bool DepthTest = true;
		bool FaceCulling = true;
		RenderCullFace CullFace = RenderCullFace::Back;
		bool UseViewport = false;
		RenderViewport Viewport;
	};

	class VertexArray;

	class FT_API RenderCommand
	{
	public:
		static void SetDepthTest(bool enabled);
		static void SetFaceCulling(bool enabled);
		static void SetCullFace(RenderCullFace face);
		static void SetViewport(const RenderViewport& viewport);
		static void SetClearColor(const glm::vec4& color);
		static void SetLineWidth(float width);
		static void Clear(const RenderClearState& clearState);
		static void DrawIndexed(const Ref<VertexArray>& vertexArray);
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);
		static bool CheckErrors(const char* label = nullptr);
	};
}
