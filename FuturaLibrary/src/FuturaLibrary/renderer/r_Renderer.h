#ifndef RENDERER_H
#define RENDERER_H 
#include "pch.h"

namespace FuturaLibrary
{
	class Renderer
	{
	public: 
		static void Initialize(); 
		static void BeginScene(/* Camera camera */);
		static void EndScene(); 
		static void Submit(/* const Ref<Shader>& shader, const glm::mat4& transform */); 

	private: 
		struct SceneData
		{
			
		};

		static SceneData* m_SceneData; 


		class WorldRenderer; 
		class BrushRenderer; 
		class SpriteRenderer; 
	};
}

#endif