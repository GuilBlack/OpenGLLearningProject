#include "pch.h"
#include "RenderCommand.h"
#include "Renderer/RendererCore.h"

void RenderCommand::Init()
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
}

void RenderCommand::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& va)
{
	va->Bind();
	glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
