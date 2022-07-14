#pragma once
#include <stdint.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void AddIndexBuffer(const IndexBuffer& ib);
	void Bind() const;
	void Unbind() const;
	inline uint32_t GetIbCount() const { return m_IbCount; }

private:
	uint32_t m_RendererID;
	uint32_t m_IbCount;
};

