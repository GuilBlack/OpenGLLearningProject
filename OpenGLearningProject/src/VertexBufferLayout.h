#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferAttribute
{
	uint32_t type;
	uint32_t count;
	bool normalized;

	static uint32_t GetSizeOfType(uint32_t type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_INT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_BYTE: return 1;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferAttribute> m_Attributes;
	uint32_t m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="count">Number of values per vertices</param>
	/// <param name="normalized">False if the value is already normalized, true otherwise.</param>
	template<typename T>
	void Push(uint32_t count, bool normalized = false) = delete;

	template<>
	void Push<float>(uint32_t count, bool normalized)
	{
		m_Attributes.push_back({ GL_FLOAT, count, normalized });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<uint32_t>(uint32_t count, bool normalized)
	{
		m_Attributes.push_back({ GL_UNSIGNED_INT, count, normalized });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_INT);
	}

	inline const std::vector<VertexBufferAttribute> GetAttributes() const { return m_Attributes; }
	inline uint32_t GetStride() const { return m_Stride; }

	/// <summary>
	/// returns layout for vertex with only 3 floats for position.
	/// </summary>
	/// <returns></returns>
	static VertexBufferLayout pLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(3);
		return layout;
	}

	/// <summary>
	/// returns layout for vertex with 3 floats for position
	/// and 3 floats for color.
	/// </summary>
	/// <returns></returns>
	static VertexBufferLayout pcLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		return layout;
	}

	/// <summary>
	/// returns layout for vertex with 3 floats for position,
	/// 3 floats for color and 2 floats for texture coordinate.
	/// </summary>
	/// <returns></returns>
	static VertexBufferLayout pctLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		return layout;
	}

	/// <summary>
	/// returns layout for vertex with 3 floats for position,
	/// 3 floats for color, 2 floats for texture coordinate
	/// and 3 floats for normal.
	/// </summary>
	/// <returns></returns>
	static VertexBufferLayout pctnLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		return layout;
	}
};