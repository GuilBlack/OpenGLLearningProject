#include "pch.h"
#include "Engine.h"
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
{
	Clear();
}

void Renderer::EndFrame()
{
	s_Renderer->m_CommandQueue.PushCommand([]()
		{
			glfwSwapBuffers(&Engine::GetEngine().GetWindow());
		});
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

void Renderer::SetClearColor(const glm::vec4& color)
{
	s_Renderer->m_CommandQueue.PushCommand([color]()
		{
			glClearColor(color.r, color.g, color.b, color.a);
		});
}

void Renderer::Clear()
{
	s_Renderer->m_CommandQueue.PushCommand([]()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		});
}

void Renderer::Render(const VertexArray& va, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	glDrawElements(GL_TRIANGLES, va.GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
}