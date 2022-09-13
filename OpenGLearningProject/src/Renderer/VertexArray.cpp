#include "pch.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
	: m_RendererID(0)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glGenVertexArrays(1, &m_RendererID);
		});
}

VertexArray::~VertexArray()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glDeleteVertexArrays(1, &m_RendererID);
		});
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, vb]()
		{
			ASSERT(vb->GetLayout().GetAttributes().size());
			Bind();
			vb->Bind();
			const auto& layout = vb->GetLayout();
			const auto& attributes = layout.GetAttributes();
			uint32_t index = 0;

			for (const auto& attribute : attributes)
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, attribute.count, attribute.type,
					attribute.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)attribute.offset);
				++index;
			}
			vb->Unbind();
			Unbind();
			m_VertexBuffers.push_back(vb);
		});
}

void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& ib)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, ib]()
		{
			Bind();
			ib->Bind();
			Unbind();
			ib->Unbind();
			m_IndexBuffer = ib;
		});
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
