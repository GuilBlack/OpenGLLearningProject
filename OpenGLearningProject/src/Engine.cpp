#include "pch.h"
#include "Engine.h"

#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Log.h"
#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

Engine* Engine::s_Engine = nullptr;

static void GLFWErrorCallback(int errorCode, const char* description)
{
	ENGINE_ERROR("GLFW Error {0}: {1}", errorCode, description);
}

Engine::Engine(int32_t openGlMajorVersion, int32_t openGlMinorVersion, int32_t width, int32_t height, const char* windowTitle, bool resizable)
	: m_cOpenGlMajorVersion(openGlMajorVersion), m_cOpenGlMinorVersion(openGlMinorVersion), m_Resizable(resizable), m_WindowData(windowTitle, width, height, width, height)
{
	s_Engine = this;

	Log::Init();
	InitGLFW();
	InitWindow(windowTitle, resizable);
	InitCallbacks();
	//InitGlew();
	Renderer::Init();

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);
	ENGINE_INFO("Engine Created.");
	ENGINE_INFO("OpenGL version: {}", (const char*)glGetString(GL_VERSION));
	ENGINE_INFO("Vendor: {}", (const char*)glGetString(GL_VENDOR));
	ENGINE_INFO("GPU: {}", (const char*)glGetString(GL_RENDERER));
}

Engine::~Engine()
{
	m_Meshes.clear();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

/// <summary>
/// This function will initialize the glfw context and setup some window hints.
/// </summary>
void Engine::InitGLFW()
{
	//initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
	}

	// Setup error callback
	glfwSetErrorCallback(GLFWErrorCallback);

	// Setup GLFW window properties

	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_cOpenGlMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_cOpenGlMinorVersion);
	// CORE_PROFILE = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility (useful for MacOS)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Engine::InitWindow(const char* windowTitle, bool resizable)
{
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, windowTitle, NULL, NULL);
	if (m_Window == nullptr)
	{
		ENGINE_ERROR("GLFW window creation failed!");
		glfwTerminate();
	}
	
	// Set OpenGL context for GLEW
	//glfwMakeContextCurrent(m_Window);

	// Set window user data
	glfwSetWindowUserPointer(m_Window, static_cast<void*>(&m_WindowData));

	ENGINE_INFO("Window for {0} app created.", windowTitle);
}


void Engine::InitCallbacks()
{
	// Set the callback of the window data to use it in different lambdas
	SetEventCallback(BIND_EVENT_FN(Engine::OnEvent));

	// Set the resize window callback
	glfwSetFramebufferSizeCallback(
		m_Window,
		[](GLFWwindow* window, int32_t fbWidth, int32_t fbHeight)
		{
			glViewport(0, 0, fbWidth, fbHeight);
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			wData.TempWidth = fbWidth;
			wData.TempHeight = fbHeight;

			WindowResizedEvent ev(fbWidth, fbHeight);
			wData.EventCallback(ev);
		});

	// Set closing window callback
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent ev;
			wData.EventCallback(ev);
		});

	// Set cursor position callback
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			MouseMovedEvent ev(xpos, ypos);
			wData.EventCallback(ev);
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent ev(xoffset, yoffset);
			wData.EventCallback(ev);
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent ev(button);
					wData.EventCallback(ev);
					break;
				}
				case GLFW_RELEASE:
					MouseButtonReleasedEvent ev(button);
					wData.EventCallback(ev);
					break;
			}
		});

	// Set key callback
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent ev(key, 0);
					wData.EventCallback(ev);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent ev(key, 1);
					wData.EventCallback(ev);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent ev(key);
					wData.EventCallback(ev);
					break;
				}
			}
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
		{
			WindowData& wData = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			KeyTypedEvent ev(codepoint);
			wData.EventCallback(ev);
		});
}

void Engine::InitGlew()
{
	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed" << std::endl;
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}

/// <summary>
/// This function will initialize some OpenGL options.
/// </summary>
void Engine::InitOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Engine::PushMesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
	glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	m_Meshes.emplace_back(std::make_shared<Mesh>(vertices, sizeOfVb, indices, nbOfIndices, layout, position, scale, rotation));
}

void Engine::PushShader(std::string filepath)
{
	std::shared_ptr<Shader> shader;
	shader.reset(new Shader(filepath));
	m_Shaders.emplace_back(shader);
}

void Engine::Run()
{
	Renderer::SetClearColor({ 0.1f, 0.1f , 0.1f , 1.0f });
	while (m_Running)
	{
		// See if any events occured
		glfwPollEvents();

		if (m_WindowData.TempWidth <= 0 || m_WindowData.TempHeight <= 0)
			continue;

		if (m_WindowData.TempWidth != m_WindowData.Width || m_WindowData.TempHeight != m_WindowData.Height)
		{
			m_WindowData.DidWindowDimensionsChange = true;
			m_WindowData.Width = m_WindowData.TempWidth;
			m_WindowData.Height = m_WindowData.TempHeight;
		}

		Renderer::BeginFrame();

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();

		m_ImGuiLayer->Begin();
		Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
			{
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			});
		m_ImGuiLayer->End();

		Renderer::EndFrame();
		m_WindowData.DidWindowDimensionsChange = false;
	}
}

void Engine::OnEvent(Event& ev)
{
	EventDispatcher ed(ev);
	ed.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Engine::SetWindowShouldClose));

	for (auto iterator = m_LayerStack.end(); iterator != m_LayerStack.begin();)
	{
		(*--iterator)->OnEvent(ev);
		if (ev.Handler)
			break;
	}
}

bool Engine::SetWindowShouldClose(WindowCloseEvent& e)
{
	m_Running = false;
	Renderer::GetRenderer().Shutdown();
	return true;
}
