#include "pch.h"
#include "IndexBuffer.h"
#include "RendererCore.h"

/// <summary>
/// DO NOT USE THIS!
/// </summary>
IndexBuffer::IndexBuffer()
	: m_Count(0)
{
	GlCall(glGenBuffers(1, &m_RendererID));
}

/// <summary>
/// 
/// </summary>
/// <param name="data">The array of indices to store in an Index Buffer</param>
/// <param name="count">Number of indices</param>
IndexBuffer::IndexBuffer(const void* data, uint32_t count)
	: m_Count(count)
{
	GlCall(glGenBuffers(1, &m_RendererID));
	Bind();
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
	Unbind();
}

IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::AddData(const void* data, uint32_t count)
{
	m_Count = count;
	Bind();
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
	Unbind();
}

void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
