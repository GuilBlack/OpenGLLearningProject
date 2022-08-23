#include "pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
	: m_IbCount(0)
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& attributes = layout.GetAttributes();
	uint32_t offset = 0;

	for (uint32_t i = 0; i < attributes.size(); ++i)
	{
		const auto& attribute = attributes[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, attribute.count, attribute.type, 
			attribute.normalized, layout.GetStride(), (const void*)offset));
		offset += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
	}
	vb.Unbind();
	Unbind();
}

void VertexArray::AddIndexBuffer(const IndexBuffer& ib)
{
	Bind();
	ib.Bind();
	m_IbCount = ib.GetCount();
	Unbind();
	ib.Unbind();
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}
