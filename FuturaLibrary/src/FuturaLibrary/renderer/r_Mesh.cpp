#include "pch.h"
#include "r_Mesh.h"

namespace FuturaLibrary
{
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
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		auto indexBuffer = CreateRef<IndexBuffer>(
			indices.data(),
			m_IndexCount
		);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	Ref<Mesh> Mesh::CreateCube()
	{
		std::vector<Vertex> vertices =
		{
			// front
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

			// back
			{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

			// left
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

			// right
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

			// top
			{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

			// bottom
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}
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

		return CreateRef<Mesh>(vertices, indices);
	}
}
