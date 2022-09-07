#include "pch.h"
#include "Renderer.h"
#include "RendererCore.h"

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

Renderer* Renderer::s_Renderer = new Renderer();

void Renderer::Init()
{
	RenderCommand::Init();
}

void Renderer::BeginFrame()
{}

void Renderer::EndFrame()
{
	s_Renderer->m_CommandQueue.Execute();
}

void Renderer::Submit(const std::shared_ptr<VertexArray>&va, const std::shared_ptr<Shader>& shader)
{
	shader->Bind();
	va->Bind();

	RenderCommand::DrawIndexed(va);

	va->Unbind();
	shader->Unbind();
}

void Renderer::Clear()
{
	glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(const VertexArray& va, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, va.GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0));
}