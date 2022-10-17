#pragma once

class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const void* data, uint32_t count);
	IndexBuffer(const std::vector<uint32_t>& data, uint32_t count);
	~IndexBuffer();

	void AddData(const void* data, uint32_t count);

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetCount() const { return m_Count; };

private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};

