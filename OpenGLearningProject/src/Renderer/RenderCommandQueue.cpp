#include "pch.h"
#include "RenderCommandQueue.h"
#include <Engine.h>

RenderCommandQueue::RenderCommandQueue()
{}

void RenderCommandQueue::Init()
{
	m_Running = true;
	m_SafeExit = false;
	m_RenderThread = std::thread(&RenderCommandQueue::Run, this, &Engine::GetEngine().GetWindow());
}

void RenderCommandQueue::Shutdown()
{
	m_Running = false;
	if (m_RenderThread.joinable())
		m_RenderThread.join();
}

RenderCommandQueue::~RenderCommandQueue()
{
	Shutdown();
}

void RenderCommandQueue::PushCommand(CommandFn fn)
{
	std::lock_guard<std::mutex> lock(m_CommandQueueLock);

	m_CommandQueue.push_back(fn);
}

void RenderCommandQueue::Execute()
{
	while (!m_CommandQueue.empty())
	{
		m_CommandQueue.front()();
		m_CommandQueue.pop_front();
	}
}

void RenderCommandQueue::NotifyBeginLoop()
{
	--m_Loop;
	m_Loop.notify_all();
}

void RenderCommandQueue::WaitForLoop()
{
	if (m_Running)
	{
		m_Loop.wait(0);
		m_Loop++;
	}
}

void RenderCommandQueue::Run(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		m_Running = false;
	}

	ENGINE_INFO("Render Thread Created");
	ENGINE_INFO("OpenGL version: {}", (const char*)glGetString(GL_VERSION));
	ENGINE_INFO("Vendor: {}", (const char*)glGetString(GL_VENDOR));
	ENGINE_INFO("GPU: {}", (const char*)glGetString(GL_RENDERER));
	int32_t texInfo;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texInfo);
	ENGINE_INFO("Max Texture Size: {}", texInfo);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texInfo);
	ENGINE_INFO("Max Texture Slots: {}", texInfo);

	while (m_Running || !m_SafeExit)
	{
		if (m_CommandQueue.empty())
			continue;

		m_CommandQueue.front()();

		std::lock_guard<std::mutex> lock(m_CommandQueueLock);
		m_CommandQueue.pop_front();
	}
}
