#include "pch.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
	: m_RendererID(0)
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb)
{
	ASSERT(vb->GetLayout().GetAttributes().size());
	Bind();
	vb->Bind();
	const auto& layout = vb->GetLayout();
	const auto& attributes = layout.GetAttributes();
	uint32_t index = 0;

	for ( const auto& attribute : attributes)
	{
		GlCall(glEnableVertexAttribArray(index));
		GlCall(glVertexAttribPointer(index, attribute.count, attribute.type, 
			attribute.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)attribute.offset));
		++index;
	}
	vb->Unbind();
	Unbind();
	m_VertexBuffers.push_back(vb);
}

void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& ib)
{
	Bind();
	ib->Bind();
	Unbind();
	ib->Unbind();
	m_IndexBuffer = ib;
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}
