#include "pch.h"
#include "g_Buffer.h"

#include <glad/glad.h>
#include "FuturaLibrary/renderer/r_Renderer.h"

namespace FuturaLibrary
{
	// Vertex Buffer
	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		FT_PROFILE_FUNCTION; 
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, vertices, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		FT_PROFILE_FUNCTION;
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind() const
	{
		FT_PROFILE_FUNCTION;
		FT_CORE_ASSERT(m_RendererID != 0, "Attempted to bind an uninitialized VertexBuffer");
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return std::make_unique<VertexBuffer>(vertices, size); 
	}

	// Index Buffer
	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		FT_PROFILE_FUNCTION; 
		glCreateBuffers(1, &m_RendererID); 
		glNamedBufferData(m_RendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		FT_PROFILE_FUNCTION; 
		glDeleteBuffers(1, &m_RendererID); 
	}

	void IndexBuffer::Bind() const
	{
		FT_PROFILE_FUNCTION; 
		FT_CORE_ASSERT(m_RendererID != 0, "Attempted to bind an uninitialized IndexBuffer");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); 
	}

	std::unique_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return std::make_unique<IndexBuffer>(indices, count); 
	}

	// Storage Buffer
	StorageBuffer::StorageBuffer(uint32_t target, const void* data, uint32_t size, uint32_t flags) : m_Target(target)
	{
		FT_PROFILE_FUNCTION; 
		glCreateBuffers(1, &m_RendererID); 
		glNamedBufferStorage(m_RendererID, size, data, flags);
	}

	StorageBuffer::~StorageBuffer()
	{
		FT_PROFILE_FUNCTION;
		glDeleteBuffers(1, &m_RendererID); 
	}

	void StorageBuffer::Bind() const
	{
		FT_PROFILE_FUNCTION; 
		FT_CORE_ASSERT(m_RendererID != 0, "Attempted to bind an uninitialized StorageBuffer");
		glBindBuffer(m_Target, m_RendererID); 
	}

	void StorageBuffer::BindBufferBase(uint32_t index)
	{
		FT_PROFILE_FUNCTION;
		FT_CORE_ASSERT(m_RendererID != 0, "Attempted to bind an uninitialized StorageBuffer");
		glBindBufferBase(m_Target, index, m_RendererID); 
	}

	void* StorageBuffer::MapBufferRange(uint32_t offset, uint32_t length,
		uint32_t access)
	{
		FT_CORE_ASSERT(m_RendererID != 0, "Attempted to map an uninitialized StorageBuffer");
		return glMapNamedBufferRange(m_RendererID, offset, length, access);
	}

	Ref<StorageBuffer> StorageBuffer::Create(uint32_t target, const void* data, uint32_t size,
		uint32_t flags)
	{
		return std::make_shared<StorageBuffer>(target, data, size, flags);
	}
}
