#include "pch.h"
#include "Engine3D.h"

#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Log.h"
#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

static void GLFWErrorCallback(int errorCode, const char* description)
{
	ENGINE_ERROR("GLFW Error {0}: {1}", errorCode, description);
}

Engine3D::Engine3D(int32_t openGlMajorVersion, int32_t openGlMinorVersion, int32_t width, int32_t height, const char* windowTitle, bool resizable)
	: m_cOpenGlMajorVersion(openGlMajorVersion), m_cOpenGlMinorVersion(openGlMinorVersion), m_Resizable(resizable), m_WindowData(windowTitle, width, height, width, height)
{
	Log::Init();
	InitGLFW();
	InitWindow(windowTitle, resizable);
	InitCallbacks();
	InitGlew();
	InitOpenGLOptions();
	ENGINE_INFO("Engine Created.");
}

Engine3D::~Engine3D()
{
	m_Meshes.clear();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

/// <summary>
/// This function will initialize the glfw context and setup some window hints.
/// </summary>
void Engine3D::InitGLFW()
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

void Engine3D::InitWindow(const char* windowTitle, bool resizable)
{
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, windowTitle, NULL, NULL);
	if (m_Window == nullptr)
	{
		ENGINE_ERROR("GLFW window creation failed!");
		glfwTerminate();
	}
	
	// Set OpenGL context for GLEW
	glfwMakeContextCurrent(m_Window);

	// Set window user data
	glfwSetWindowUserPointer(m_Window, static_cast<void*>(&m_WindowData));

	ENGINE_INFO("Window for {0} app created.", windowTitle);
}


void Engine3D::InitCallbacks()
{
	// Set the callback of the window data to use it in different lambdas
	SetEventCallback(BIND_EVENT_FN(Engine3D::OnEvent));

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
}

void Engine3D::InitGlew()
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
void Engine3D::InitOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Engine3D::PushMesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
	glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	m_Meshes.emplace_back(std::make_shared<Mesh>(vertices, sizeOfVb, indices, nbOfIndices, layout, position, scale, rotation));
}

void Engine3D::PushShader(std::string filepath)
{
	m_Shaders.emplace_back(std::make_shared<Shader>(filepath));
}

void Engine3D::Run()
{
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)m_WindowData.Width / (GLfloat)m_WindowData.Height, 0.1f, 500.0f);
	m_Shaders[0]->Bind();
	m_Shaders[0]->SetUniformMatrix4fv("projection", projection);
	while (m_Running)
	{
		// See if any events occured
		glfwPollEvents();

		if (m_WindowData.TempWidth != m_WindowData.Width || m_WindowData.TempHeight != m_WindowData.Height)
		{
			m_WindowData.Width = m_WindowData.TempWidth;
			m_WindowData.Height = m_WindowData.TempHeight;
			glm::mat4 projection = glm::perspective(45.0f, (GLfloat)m_WindowData.Width / (GLfloat)m_WindowData.Height, 0.1f, 500.0f);
			m_Shaders[0]->Bind();
			m_Shaders[0]->SetUniformMatrix4fv("projection", projection);
		}

		m_Renderer.Clean();
		
		m_Meshes[0]->Rotate(glm::vec3(0.0f, 0.2f, 0.0f));
		m_Meshes[0]->Draw(m_Shaders[0]);

		glfwSwapBuffers(m_Window);
	}
}

void Engine3D::OnEvent(Event& e)
{
	ENGINE_TRACE("{0}", e);
	EventDispatcher ed(e);
	ed.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Engine3D::SetWindowShouldClose));
}

bool Engine3D::SetWindowShouldClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}
