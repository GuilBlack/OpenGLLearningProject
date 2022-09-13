#include "pch.h"
#include "IndexBuffer.h"
#include "Renderer.h"

/// <summary>
/// DO NOT USE THIS!
/// </summary>
IndexBuffer::IndexBuffer()
	: m_RendererID(0), m_Count(0)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glGenBuffers(1, &m_RendererID);
		});
}

/// <summary>
/// 
/// </summary>
/// <param name="data">The array of indices to store in an Index Buffer</param>
/// <param name="count">Number of indices</param>
IndexBuffer::IndexBuffer(const void* data, uint32_t count)
	: m_RendererID(0), m_Count(count)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, data]()
		{
			glGenBuffers(1, &m_RendererID);
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), data, GL_STATIC_DRAW);
			Unbind();
		});
}

IndexBuffer::~IndexBuffer()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glDeleteBuffers(1, &m_RendererID);
		});
}

void IndexBuffer::AddData(const void* data, uint32_t count)
{
	m_Count = count;
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, data]()
		{
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), data, GL_STATIC_DRAW);
			Unbind();
		});
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
