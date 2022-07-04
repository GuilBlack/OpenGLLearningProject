#pragma once
#include <stdint.h>

class IndexBuffer
{
private:
	uint32_t m_RendererID;
	uint32_t m_Count;

public:
	IndexBuffer();
	IndexBuffer(const void* data, uint32_t count);
	~IndexBuffer();

	void AddData(const void* data, uint32_t count);

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetCount() const { return m_Count; };
};

