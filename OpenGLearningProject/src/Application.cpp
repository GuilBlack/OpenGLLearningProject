#pragma once
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Mesh.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#ifndef PR_TO_RADIANS
#define PR_TO_RADIANS (float)M_PI / 180.0f
#endif // !PR_TO_RADIANS

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
int32_t bufferWidth, bufferHeight;
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.8f;
float triIncrement = 0.005f;
float curAngle = 0.0f;

void framebufferResizeCallback(GLFWwindow* window, int32_t fbWidth, int32_t fbHeight)
{
	bufferWidth = fbWidth;
	bufferHeight = fbHeight;
	glViewport(0, 0, fbWidth, fbHeight);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW initialisation failed!" << std::endl;
		return -1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Get Buffer size info
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW init failed" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}

	//OPENGL Options
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Setup Viewport size
	glfwSetFramebufferSizeCallback(mainWindow, framebufferResizeCallback);

	{
		std::vector<uint32_t> indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 2, 1
		};

		std::vector<float> vertices = {
			-1.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		Renderer renderer;
		Shader shader("res/shaders/Basic.shader");

		VertexArray va;
		VertexBuffer vb(vertices.data(), vertices.size() * sizeof(vertices[0]));
		IndexBuffer ib(indices.data(), indices.size());
		VertexBufferLayout layout;
		layout.Push<float>(3);

		va.AddIndexBuffer(ib);
		va.AddVertexBuffer(vb, layout);

		Mesh mesh(vertices.data(), vertices.size() * sizeof(vertices[0]),
			indices.data(), indices.size(), VertexBufferLayout::pLayout(),
			glm::vec3(0.0f, 0.0f, -3.0f),
			glm::vec3(0.4f, 0.4f, 0.4f));

		glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 500.0f);

		// Loop until window closed
		while (!glfwWindowShouldClose(mainWindow))
		{
			// Get + Handle user input events
			glfwPollEvents();
			processInput(mainWindow);

			// Clear the window
			renderer.Clean();

			shader.Bind();

			mesh.Rotate(glm::vec3(0.0f, 0.2f, 0.0f));

			shader.SetUniformMatrix4fv("projection", projection);

			mesh.Draw(shader);

			glfwSwapBuffers(mainWindow);
		}
	}


	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}