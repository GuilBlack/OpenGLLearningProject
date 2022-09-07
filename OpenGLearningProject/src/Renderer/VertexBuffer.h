#pragma once
#include "VertexBufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void AddData(const void* data, uint32_t size);

	void Bind() const;
	void Unbind() const;

	const VertexBufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }

private:
	uint32_t m_RendererId;
	uint32_t m_Size;
	VertexBufferLayout m_Layout;
};

