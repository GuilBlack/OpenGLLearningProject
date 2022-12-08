#include "pch.h"
#include "Texture.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"


static GLenum GetGlTextureFormat(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::RGB:	return GL_RGB;
		case TextureFormat::RGBA:	return GL_RGBA;
	}
	return 0;
}

static GLenum GetGlTextureSizedFormat(TextureFormat format, bool srgb)
{
	switch (format)
	{
		case TextureFormat::RGB:	return srgb ? GL_SRGB8 : GL_RGB8;
		case TextureFormat::RGBA:	return srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
	}
	return 0;
}

Texture2D::Texture2D(TextureFormat format, uint32_t width, uint32_t height, bool isSRGB)
	: m_RendererID(0), m_TextureFormat(format), m_Width(width), m_Height(height), m_IsSRGB(isSRGB), m_ImageData(nullptr)
{
	Renderer::GetRendererCommandQueue().PushCommand([this]()
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			glTexImage2D(GL_TEXTURE_2D, 0, GetGlTextureFormat(m_TextureFormat), m_Width, m_Height, 0, GetGlTextureFormat(m_TextureFormat), GL_UNSIGNED_BYTE, nullptr);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		});
}

Texture2D::Texture2D(const std::string & path, bool isSRGB)
	: m_RendererID(0), m_Path(path), m_TextureFormat(), m_Width(0), m_Height(0), m_IsSRGB(isSRGB), m_ImageData(nullptr)
{
	int32_t format;
	
	stbi_set_flip_vertically_on_load(false);
	m_ImageData = stbi_load(path.c_str(), &m_Width, &m_Height, &format, 0);
	if (stbi_failure_reason() && m_ImageData == NULL && !(format == 3 || format == 4))
	{
		ENGINE_ERROR("TextureError: {} {}", stbi_failure_reason(), m_Path);
		stbi_image_free(m_ImageData);
		return;
	}
	ASSERT((format == 3 || format == 4) && "Doesn't support greyscale yet!");
	m_TextureFormat = format == 3 ? TextureFormat::RGB : TextureFormat::RGBA;

	Renderer::GetRendererCommandQueue().PushCommand([this]()
		{
			uint32_t levels = 1;

			//m_Width | m_Height will make it so that this dimension will always be >= the biggest of the 2
			//>> level will divide it by 2 till we reach the smallest mipmap possible (1x1)
			while ((m_Width | m_Height) >> levels)
			{
				++levels;
			}

			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);

			glTexStorage2D(GL_TEXTURE_2D, levels, GetGlTextureSizedFormat(m_TextureFormat, m_IsSRGB), m_Width, m_Height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//int32_t format;
			//glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &format);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GetGlTextureFormat(m_TextureFormat), GL_UNSIGNED_BYTE, m_ImageData);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		});
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
	stbi_image_free(m_ImageData);
}

Texture2D* Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, bool isSRGB)
{
	return nullptr;
}

Texture2D* Texture2D::Create(const std::string& path, bool isSRGB)
{
	return new Texture2D(path, isSRGB);
}

void Texture2D::Bind(uint32_t slot)
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glActiveTexture(GL_TEXTURE0 + slot);
}
