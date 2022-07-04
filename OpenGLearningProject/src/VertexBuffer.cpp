#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer()
	: m_Size(0)
{
	GlCall(glGenBuffers(1, &m_RendererId));
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Array of vertices to pass to the vertex buffer</param>
/// <param name="size">Size of the array in bytes</param>
VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	: m_Size(size)
{
	GlCall(glGenBuffers(1, &m_RendererId));
	Bind();
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	Unbind();
}

VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererId));
}

void VertexBuffer::AddData(const void* data, uint32_t size)
{
	m_Size = size;
	Bind();
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	Unbind();
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
