#pragma once
#include <stdint.h>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void AddData(const void* data, uint32_t size);

	void Bind() const;
	void Unbind() const;

private:
	uint32_t m_RendererId;
	uint32_t m_Size;
};

