#pragma once

#include "pch.h"
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
		case ShaderDataType::Mat4:	 return 4 * 3 * 4;
		case ShaderDataType::Int:	 return 4; 
		case ShaderDataType::Int2:	 return 4 * 2;
		case ShaderDataType::Int3:   return 4 * 3;
		case ShaderDataType::Int4:	 return 4 * 4;
		case ShaderDataType::Bool:	 return 1; 
		}
		FT_CORE_ASSERT(false, "Unknown ShaderDataType"); 
		return 0; 
	}
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

	// Buffer Layout
	// Vertex Buffer
	// Index Buffer
	// Storage Buffer






}