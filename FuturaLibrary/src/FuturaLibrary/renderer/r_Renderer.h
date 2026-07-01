#ifndef RENDERER_H
#define RENDERER_H 
#include "pch.h"

#include "FuturaLibrary/graphics/g_Shader.h"
#include "FuturaLibrary/graphics/g_VertexArray.h"
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	class FT_API Renderer
	{
	public: 
		static void Initialize(); 
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene(); 
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform); 

	private: 
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
		};

		static SceneData* m_SceneData; 


		class WorldRenderer; 
		class BrushRenderer; 
		class SpriteRenderer; 
	};
}

#endif
