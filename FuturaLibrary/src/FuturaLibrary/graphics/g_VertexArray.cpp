#include "pch.h"
#include "g_VertexArray.h"

#include <glad/glad.h>

namespace FuturaLibrary
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_BOOL;
		}
		FT_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0; 
	}

    VertexArray::VertexArray()
    {
        FT_PROFILE_FUNCTION; 
        glCreateVertexArrays(1, &m_RendererID); 
    }

    VertexArray::~VertexArray()
    {
        FT_PROFILE_FUNCTION; 
        glDeleteVertexArrays(1, &m_RendererID); 
    }

    void VertexArray::Bind() const
    {
        FT_PROFILE_FUNCTION; 
        FT_CORE_ASSERT(m_RendererID != 0, "Attempted to bind an uninitialized value");
        glBindVertexArray(m_RendererID);
    }

    // Adds a vertex buffer to the VAO and configures its attribute layout
    void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        FT_PROFILE_FUNCTION;
        FT_CORE_ASSERT(vertexBuffer, "VertexBuffer is nullptr");
        FT_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!");

        const auto& layout = vertexBuffer->GetLayout();
        uint32_t bindIndex = m_VertexBuffers.size();

        glVertexArrayVertexBuffer(
            m_RendererID,
            bindIndex,
            vertexBuffer->GetID(),
            0,
            layout.GetStride()
            );

        for (const auto& element : layout)
        {
            glEnableVertexArrayAttrib(m_RendererID, m_AttribIndex);

            glVertexArrayAttribFormat(
                m_RendererID,
                m_AttribIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                element.Offset
            );

            glVertexArrayAttribBinding(
                m_RendererID, 
                m_AttribIndex,
                bindIndex
            );
            
            m_AttribIndex++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    // Sets the index buffer for the VAO to define vertex drawing order
    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        FT_PROFILE_FUNCTION;
        FT_CORE_ASSERT(indexBuffer, "IndexBuffer is nullptr");

        glVertexArrayElementBuffer(
            m_RendererID,
            indexBuffer->GetID()
        );

        m_IndexBuffer = indexBuffer; 
    }
}