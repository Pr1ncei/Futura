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

		const char* GLErrorToString(GLenum error)
		{
			switch (error)
			{
				case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
				case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
				case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
				case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
				case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
			}

			return "Unknown OpenGL error";
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

	void RenderCommand::SetLineWidth(float width)
	{
		glLineWidth(width);
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

	void RenderCommand::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		FT_CORE_ASSERT(vertexArray, "RenderCommand::DrawLines received a null vertex array!");
		if (vertexCount == 0)
			return;

		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertexCount));
	}

	bool RenderCommand::CheckErrors(const char* label)
	{
		bool foundError = false;
		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			foundError = true;
			if (label)
				FT_CORE_ERROR("OpenGL error after {0}: {1} ({2})", label, GLErrorToString(error), error);
			else
				FT_CORE_ERROR("OpenGL error: {0} ({1})", GLErrorToString(error), error);

			error = glGetError();
		}

		return foundError;
	}
}
