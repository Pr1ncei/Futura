#include "pch.h"
#include "r_Mesh.h"

namespace FuturaLibrary
{
	namespace
	{
		Vertex CreateVertex(
			const glm::vec3& position,
			const glm::vec2& texCoord,
			const glm::vec3& normal
		)
		{
			Vertex vertex;
			vertex.Position = position;
			vertex.TexCoord = texCoord;
			vertex.Normal = normal;
			vertex.LightmapTexCoord = texCoord;
			return vertex;
		}
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		: m_IndexCount(static_cast<uint32_t>(indices.size()))
	{
		FT_CORE_ASSERT(!vertices.empty(), "Mesh requires vertices!");
		FT_CORE_ASSERT(!indices.empty(), "Mesh requires indices!");

		m_VertexArray = CreateRef<VertexArray>();

		auto vertexBuffer = CreateRef<VertexBuffer>(
			vertices.data(),
			static_cast<uint32_t>(vertices.size() * sizeof(Vertex))
		);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_LightmapTexCoord" }
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = CreateRef<IndexBuffer>(
			indices.data(),
			m_IndexCount
		);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	Mesh::Mesh(const MeshData& meshData)
		: Mesh(meshData.Vertices, meshData.Indices)
	{
	}

	Ref<Mesh> Mesh::Create(const MeshData& meshData)
	{
		return CreateRef<Mesh>(meshData);
	}

	Ref<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		return CreateRef<Mesh>(vertices, indices);
	}

	Ref<Mesh> Mesh::CreateCube()
	{
		std::vector<Vertex> vertices =
		{
			// front
			CreateVertex({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}),
			CreateVertex({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}),
			CreateVertex({ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}),
			CreateVertex({-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}),

			// back
			CreateVertex({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}),
			CreateVertex({-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}),
			CreateVertex({-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}),
			CreateVertex({ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}),

			// left
			CreateVertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}),
			CreateVertex({-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}),
			CreateVertex({-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}),
			CreateVertex({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}),

			// right
			CreateVertex({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}),
			CreateVertex({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}),
			CreateVertex({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}),
			CreateVertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}),

			// top
			CreateVertex({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}),
			CreateVertex({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}),
			CreateVertex({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}),
			CreateVertex({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}),

			// bottom
			CreateVertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}),
			CreateVertex({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}),
			CreateVertex({ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}),
			CreateVertex({-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f})
		};

		std::vector<uint32_t> indices =
		{
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		return Create(vertices, indices);
	}
}
