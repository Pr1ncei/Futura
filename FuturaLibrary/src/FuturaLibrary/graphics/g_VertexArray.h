#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "FuturaLibrary/graphics/g_Buffer.h"

namespace FuturaLibrary
{	

	// Container that groups vertex buffers and optionally adds index buffer into a single object
	// that the GPU can use for drawing. 
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete; 
		VertexArray& operator=(const VertexArray&) = delete; 

		VertexArray(VertexArray&&) noexcept = default;
		VertexArray& operator=(VertexArray&&) noexcept = default; 

		inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const	{ return m_VertexBuffer; }
		inline const Ref<IndexBuffer>& GetIndexBuffer() const					{ return m_IndexBuffer; }

		void Bind() const; 
		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer); 
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer); 

		static std::unique_ptr<VertexArray> Create() { return std::make_unique<VertexArray>(); }

	private: 
		uint32_t m_RendererID = 0; 
		uint32_t m_AttribIndex = 0;  // tracks the next available attribute index

		std::vector<Ref<VertexBuffer>> m_VertexBuffers; 
		Ref<IndexBuffer> m_IndexBuffer; 
	};
}

#endif 