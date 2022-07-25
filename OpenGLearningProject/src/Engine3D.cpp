#include "Engine3D.h"
#include <iostream>

#include "vendor/glm/gtc/matrix_transform.hpp"

#define GLFW_INIT_ERROR 69
#define CREATE_WINDOW_ERROR 96
#define GLEW_INIT_ERROR 420

Engine3D::Engine3D(int32_t openGlMajorVersion, int32_t openGlMinorVersion, int32_t width, int32_t height, const char* windowTitle, bool resizable)
	: m_cOpenGlMajorVersion(openGlMajorVersion), m_cOpenGlMinorVersion(openGlMinorVersion), m_Width(width), m_Height(height), m_BufferWidth(width), m_BufferHeight(height), m_Resizable(resizable)
{
	InitGlfw();
	InitWindow(windowTitle, resizable);
	InitGlew();
	InitOpenGLOptions();
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
void Engine3D::InitGlfw()
{
	//initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
	}

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

	m_Window = glfwCreateWindow(m_Width, m_Height, windowTitle, NULL, NULL);
	if (m_Window == nullptr)
	{
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
	}
	
	// Set OpenGL context for GLEW
	glfwMakeContextCurrent(m_Window);

	// Set the resize window callback
	glfwSetFramebufferSizeCallback(
		m_Window, 
		[](GLFWwindow* window, int32_t fbWidth, int32_t fbHeight)
		{
			glViewport(0, 0, fbWidth, fbHeight);
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
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)m_Width / (GLfloat)m_Height, 0.1f, 500.0f);
	m_Shaders[0]->Bind();
	m_Shaders[0]->SetUniformMatrix4fv("projection", projection);
	while (!glfwWindowShouldClose(m_Window))
	{
		if (m_Resizable)
		{
			glfwGetFramebufferSize(m_Window, &m_BufferWidth, &m_BufferHeight);
			if (m_Height != m_BufferHeight || m_Width != m_BufferWidth)
			{
				m_Height = m_BufferHeight;
				m_Width = m_BufferWidth;
				glm::mat4 projection = glm::perspective(45.0f, (GLfloat)m_Width / (GLfloat)m_Height, 0.1f, 500.0f);
				m_Shaders[0]->Bind();
				m_Shaders[0]->SetUniformMatrix4fv("projection", projection);
			}
		}

		glfwPollEvents();

		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			SetWindowShouldClose();

		m_Renderer.Clean();
		
		m_Meshes[0]->Rotate(glm::vec3(0.0f, 0.2f, 0.0f));
		m_Meshes[0]->Draw(m_Shaders[0]);

		glfwSwapBuffers(m_Window);
	}
}

void Engine3D::SetWindowShouldClose()
{
	glfwSetWindowShouldClose(m_Window, true);
}
