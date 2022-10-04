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

//Renderer* Renderer::s_Renderer = new Renderer();

void Renderer::Init()
{
	GetRenderer().GetCommandQueue().PushCommand([]()
		{
#ifdef DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			glDebugMessageCallback([](
				uint32_t source,
				uint32_t type,
				uint32_t id,
				uint32_t severity,
				int32_t length,
				const char* message,
				const void* userParam)
				{
					switch (severity)
					{
						case GL_DEBUG_SEVERITY_HIGH:         ENGINE_CRITICAL(message); return;
						case GL_DEBUG_SEVERITY_MEDIUM:       ENGINE_ERROR(message); return;
						case GL_DEBUG_SEVERITY_LOW:          ENGINE_WARN(message); return;
						case GL_DEBUG_SEVERITY_NOTIFICATION: ENGINE_INFO(message); return;
					}

					ASSERT(false);
				},
				nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_LINE_SMOOTH);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);

			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		});
}

void Renderer::Shutdown()
{
	m_CommandQueue.Shutdown();
}

void Renderer::BeginFrame()
{
	GetRenderer().GetCommandQueue().PushCommand([]()
		{
			RenderCommandQueue& queue = GetRenderer().GetCommandQueue();
			queue.SetSafeExit(false);
			queue.NotifyBeginLoop();
		});
	Clear();
}

void Renderer::EndFrame()
{
	RenderCommandQueue& queue = GetRenderer().GetCommandQueue();
	queue.PushCommand([]()
		{
			glfwSwapBuffers(&Engine::GetEngine().GetWindow());
			GetRenderer().GetCommandQueue().SetSafeExit(true);
		});
	queue.WaitForLoop();
	//s_Renderer->m_CommandQueue.Execute();
}

void Renderer::Submit(const std::shared_ptr<VertexArray>&va, const std::shared_ptr<Shader>& shader)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([va, shader]()
		{
			shader->Bind();
			va->Bind();

			glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			va->Unbind();
			shader->Unbind();
		});
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	GetRenderer().m_CommandQueue.PushCommand([color]()
		{
			glClearColor(color.r, color.g, color.b, color.a);
		});
}

void Renderer::Clear()
{
	GetRenderer().m_CommandQueue.PushCommand([]()
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