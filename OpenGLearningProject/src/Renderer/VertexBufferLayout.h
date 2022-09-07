#pragma once

#include <vector>
#include "RendererCore.h"

struct VertexBufferAttribute
{
	std::string name;
	uint32_t type;
	uint32_t count;
	uint32_t offset;
	bool normalized;

	VertexBufferAttribute(const std::string& n, uint32_t t, uint32_t c, uint32_t o = 0, bool norm = false)
		: name(n), type(t), count(c), offset(o), normalized(norm) {}

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
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	VertexBufferLayout(const std::vector<VertexBufferAttribute>& attributes)
		: m_Stride(0), m_Attributes(attributes)
	{
		uint32_t offset = 0;
		for (auto& attribute : m_Attributes)
		{
			attribute.offset = offset;
			m_Stride += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
			offset += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="count">Number of values per vertices</param>
	/// <param name="normalized">False if the value is already normalized, true otherwise.</param>
	template<typename T>
	void Push(const std::string name, uint32_t count, bool normalized = false) = delete;

	template<>
	void Push<float>(const std::string name, uint32_t count, bool normalized)
	{
		m_Attributes.push_back({ name, GL_FLOAT, count, m_Stride, normalized });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<uint32_t>(const std::string name, uint32_t count, bool normalized)
	{
		m_Attributes.push_back({ name, GL_UNSIGNED_INT, count, m_Stride, normalized });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_INT);
	}

	inline const std::vector<VertexBufferAttribute> GetAttributes() const { return m_Attributes; }
	inline uint32_t GetStride() const { return m_Stride; }

private:
	std::vector<VertexBufferAttribute> m_Attributes;
	uint32_t m_Stride;
};