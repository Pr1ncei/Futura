#pragma once

#include "FuturaLibrary/core/c_core.h"
#include "FuturaLibrary/graphics/g_VertexArray.h"
#include <glm/glm.hpp>

namespace FuturaLibrary
{
	struct Vertex
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec2 TexCoord = glm::vec2(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec2 LightmapTexCoord = glm::vec2(0.0f);
	};

	struct AxisAlignedBounds
	{
		glm::vec3 Min = glm::vec3(0.0f);
		glm::vec3 Max = glm::vec3(0.0f);
		bool IsValid = false;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		AxisAlignedBounds LocalBounds;
	};

	FT_API AxisAlignedBounds CalculateMeshBounds(const std::vector<Vertex>& vertices);

	class FT_API Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const MeshData& meshData);

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const AxisAlignedBounds& GetLocalBounds() const { return m_LocalBounds; }
		uint32_t GetIndexCount() const { return m_IndexCount; }
		uint32_t GetTriangleCount() const { return m_IndexCount / 3; }

		static Ref<Mesh> Create(const MeshData& meshData);
		static Ref<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		static Ref<Mesh> CreateCube();

	private:
		void InitializeBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		Ref<VertexArray> m_VertexArray;
		AxisAlignedBounds m_LocalBounds;
		uint32_t m_IndexCount = 0;
	};
}
