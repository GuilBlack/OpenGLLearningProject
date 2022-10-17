#include "pch.h"
#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer()
	: m_RendererID(0), m_Size(0)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glGenBuffers(1, &m_RendererID);
		});
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Array of vertices to pass to the vertex buffer</param>
/// <param name="size">Size of the array in bytes</param>
VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	: m_RendererID(0), m_Size(size)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, data]()
		{
			glGenBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, m_Size, data, GL_STATIC_DRAW);
			Unbind();
		});
}

/// <summary>
/// 
/// </summary>
/// <param name="data">Vector of vertices to pass to the vertex buffer</param>
/// <param name="size">Size of the array in bytes</param>
VertexBuffer::VertexBuffer(const std::vector<float>& data, uint32_t size)
	: m_RendererID(0), m_Size(size)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, data]()
		{
			glGenBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ARRAY_BUFFER, m_Size, data.data(), GL_STATIC_DRAW);
			Unbind();
		});
}

VertexBuffer::~VertexBuffer()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glDeleteBuffers(1, &m_RendererID);
		});
}

void VertexBuffer::AddData(const void* data, uint32_t size)
{
	m_Size = size;
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, data]()
		{
			Bind();
			glBufferData(GL_ARRAY_BUFFER, m_Size, data, GL_STATIC_DRAW);
			Unbind();
		});
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
