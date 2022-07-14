#include "Renderer.h"
#include <iostream>

void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clean() const
{
	glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(const VertexArray& va, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, va.GetIbCount(), GL_UNSIGNED_INT, 0));
}
