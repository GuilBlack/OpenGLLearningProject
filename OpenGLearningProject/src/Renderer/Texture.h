#pragma once

enum class TextureFormat
{
	None = 0,
	RGB = 1,
	RGBA = 2
};

class Texture2D
{
public:
	Texture2D(TextureFormat format, uint32_t width, uint32_t height, bool isSRGB);
	Texture2D(const std::string& path, bool isSRGB);
	~Texture2D();

	static Texture2D* Create(TextureFormat format, uint32_t width, uint32_t height, bool isSRGB = false);
	static Texture2D* Create(const std::string& path, bool isSRGB = false);

	void Bind(uint32_t slot = 0);

	inline uint32_t GetWidth() { return m_Width; }
	inline uint32_t GetHeight() { return m_Height; }
	inline TextureFormat GetTextureFormat() { return m_TextureFormat; }

	inline uint32_t GetRendererID() const { return m_RendererID; }

private:
	uint32_t m_RendererID;
	std::string m_Path;

	TextureFormat m_TextureFormat;
	int32_t m_Width, m_Height;
	bool m_IsSRGB;

	unsigned char* m_ImageData;
};

