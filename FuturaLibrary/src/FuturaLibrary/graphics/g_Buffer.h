#pragma once

#include "pch.h"
#include "FuturaLibrary/core/c_core.h"
namespace FuturaLibrary
{
	enum class ShaderDataType
	{
		None = 0, 
		Float,		// Scalar Value
		Float2,		// UV Coords, 2D pos
		Float3,		// Position, normal, tangent
		Float4,		// Color RGBA, weights
		Mat3,		// Normal Rotation
		Mat4,		// Model/View/Projection
		Int,		// Material ID, object ID 
		Int2,		// Texture Atlas Coords
		Int3,		// Bone IDs
		Int4,		// Bone IDs, Cluster IDs
		Bool		// Flags 
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:	 return 4; 
		case ShaderDataType::Float2: return 4 * 2; 
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3:	 return 4 * 3 * 3; 
		case ShaderDataType::Mat4:	 return 4 * 4 * 4;
		case ShaderDataType::Int:	 return 4; 
		case ShaderDataType::Int2:	 return 4 * 2;
		case ShaderDataType::Int3:   return 4 * 3;
		case ShaderDataType::Int4:	 return 4 * 4;
		case ShaderDataType::Bool:	 return 1; 
		}
		FT_CORE_ASSERT(false, "Unknown ShaderDataType"); 
		return 0; 
	}

	// Attribute of a vertex in a vertex buffer 
	struct BufferElement
	{
		std::string Name; 
		ShaderDataType Type; 
		uint32_t Size; 
		uint32_t Offset; 
		bool Normalized; 
		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) 
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) { }
		uint32_t GetComponentCount() const
		{
			switch (Type) {
			case ShaderDataType::Float: case ShaderDataType::Int: case ShaderDataType::Bool: return 1;
			case ShaderDataType::Float2: case ShaderDataType::Int2: return 2;
			case ShaderDataType::Float3: case ShaderDataType::Int3: return 3;
			case ShaderDataType::Float4: case ShaderDataType::Int4: return 4;
			case ShaderDataType::Mat3: return 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4;
			default:
				FT_CORE_ASSERT(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	};
	
	// Full Layout of a Vertex 
	class BufferLayout
	{
	public: 
		BufferLayout() { }

		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetAndStride(); 
		}

		inline const std::vector<BufferElement>& GetElements() const		{ return m_Elements; }
		inline uint32_t GetStride() const									{ return m_Stride; }
		std::vector<BufferElement>::iterator begin()						{ return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end()							{ return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const			{ return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const				{ return m_Elements.end(); }
	
	private: 
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0; 
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset; 
				offset += element.Size; 
				m_Stride += element.Size; 
			}
		}
		
		std::vector<BufferElement> m_Elements; 
		uint32_t m_Stride = 0; 
	};

	// Wraps an OpenGL VBO and stores vertex data in GPU mem
	class FT_API VertexBuffer
	{
	public: 
		VertexBuffer(float* vertices, uint32_t count); 
		~VertexBuffer(); 
		
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&&) noexcept = default; 
		VertexBuffer& operator=(VertexBuffer&&) noexcept = default; 
		
		inline void SetLayout(const BufferLayout& layout)	{ m_Layout = layout; }
		uint32_t GetID() const								{ return m_RendererID; }
		inline const BufferLayout& GetLayout() const		{ return m_Layout; }
		
		void Bind() const;
		static std::unique_ptr<VertexBuffer> Create(float* vertices, uint32_t count); 

	private:
		uint32_t m_RendererID; 
		BufferLayout m_Layout; 	
	};

	// Wraps OpenGL EBO and stores indices for drawing vertices efficiently 
	class FT_API IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count); 
		~IndexBuffer(); 

		IndexBuffer(const IndexBuffer&) = delete; 
		IndexBuffer& operator=(const IndexBuffer&) = delete; 

		IndexBuffer(IndexBuffer&&) noexcept = default; 
		IndexBuffer& operator=(IndexBuffer&&) noexcept = default; 

		uint32_t GetCount() const	{ return m_Count; }
		uint32_t GetID() const		{ return m_RendererID; }

		void Bind() const;
		static std::unique_ptr<IndexBuffer> Create(uint32_t* indices, uint32_t size); 

	private: 
		uint32_t m_RendererID; 
		uint32_t m_Count; 
	};

	// Wraps OpenGL SSBO and can store large, arbitrary data accessible in shaders
	// Quite useful for bsp nodes, lights, and particle data 
	class FT_API StorageBuffer
	{
	public:
		StorageBuffer(uint32_t target, const void* data, uint32_t size, uint32_t flags); 
		~StorageBuffer(); 

		StorageBuffer(const StorageBuffer&) = delete; 
		StorageBuffer& operator=(const StorageBuffer&) = delete; 

		StorageBuffer(StorageBuffer&&) noexcept = default; 
		StorageBuffer& operator=(StorageBuffer&&) noexcept = default; 

		void Bind() const;
		void BindBufferBase(uint32_t index);
		void* MapBufferRange(uint32_t offset, uint32_t length, uint32_t access);
		static Ref<StorageBuffer> Create(uint32_t target, const void* data, uint32_t size, uint32_t flags);

	private: 
		uint32_t m_RendererID; 
		uint32_t m_Target; 
	};

}
