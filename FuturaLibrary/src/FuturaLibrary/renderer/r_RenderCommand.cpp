/*
	r_RenderCommand is a small wrapper around raw OpenGL commands. 
	Instead of writing OpenGL commands everything like:
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		etc..

	we centralize it with RenderCommand so we talk in engine terms 
	not raw OpenGL terms. 
*/

#include "pch.h"
#include "r_RenderCommand.h"

#include "FuturaLibrary/graphics/g_VertexArray.h"

#include <glad/glad.h>

namespace FuturaLibrary
{
	namespace
	{
		// 3D rendering with OpenGL, most objects have triangles and 
		// triangles has two sides (front and back)
		// OpenGL decides which side is front based on the order of 
		// triangle's vertices. 

		// CCW = Front Face
		// CW = Back Face 

		// The enum just tells on what OpenGL should not draw in terms of sides.
		// Reference: https://learnopengl.com/Advanced-OpenGL/Face-culling
		GLenum ToGLCullFace(RenderCullFace face)
		{
			switch (face)
			{
				case RenderCullFace::Back: return GL_BACK;
				case RenderCullFace::Front: return GL_FRONT;
				case RenderCullFace::FrontAndBack: return GL_FRONT_AND_BACK;
			}

			FT_CORE_ASSERT(false, "Unknown render cull face!");
			return GL_BACK;
		}
	}

	// Depth testing lets closer pixels hide farther pixels
	// This exists to avoid objects that can appear on top of a near cube
	// just because the near cube was drawn later. 

	// Reference: https://learnopengl.com/Advanced-OpenGL/Depth-testing
	void RenderCommand::SetDepthTest(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void RenderCommand::SetFaceCulling(bool enabled)
	{
		if (enabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}

	void RenderCommand::SetCullFace(RenderCullFace face)
	{
		glCullFace(ToGLCullFace(face));
	}

	void RenderCommand::SetViewport(const RenderViewport& viewport)
	{
		glViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderCommand::Clear(const RenderClearState& clearState)
	{
		SetClearColor(clearState.Color);

		GLbitfield clearMask = 0;
		if (clearState.ClearColor)
			clearMask |= GL_COLOR_BUFFER_BIT;
		if (clearState.ClearDepth)
			clearMask |= GL_DEPTH_BUFFER_BIT;

		if (clearMask != 0)
			glClear(clearMask);
	}

	// Draws the geometry using an index buffer 
	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		FT_CORE_ASSERT(vertexArray, "RenderCommand::DrawIndexed received a null vertex array!");

		vertexArray->Bind();

		const Ref<IndexBuffer>& indexBuffer = vertexArray->GetIndexBuffer();
		FT_CORE_ASSERT(indexBuffer, "RenderCommand::DrawIndexed currently requires an index buffer!");
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
