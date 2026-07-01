#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_VertexArray.h"
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	class FT_API Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetIndexCount() const { return m_IndexCount; }

		static Ref<Mesh> CreateCube();

	private:
		Ref<VertexArray> m_VertexArray;
		uint32_t m_IndexCount = 0;
	};
}
