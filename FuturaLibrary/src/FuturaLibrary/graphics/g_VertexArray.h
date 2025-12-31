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

		void Bind() const; 
		void Unbind() const; 

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer); 
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer); 

		inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const	{ return m_VertexBuffer; }
		inline const Ref<IndexBuffer>& GetIndexBuffer() const					{ return m_IndexBuffer; }

		static VertexArray* Create();

	private: 
		uint32_t m_RendererID; 
		std::vector<Ref<VertexBuffer>> m_VertexBuffer; 
		Ref<IndexBuffer> m_IndexBuffer; 
	};
}

#endif 