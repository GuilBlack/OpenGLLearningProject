#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb);
	void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& ib);
	void Bind() const;
	void Unbind() const;
	//inline uint32_t GetIbCount() const { return m_IbCount; }
	inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

private:
	uint32_t m_RendererID;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	//uint32_t m_IbCount;
};

