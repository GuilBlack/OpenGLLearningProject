#pragma once
#include <vector>
#include <memory>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Mesh.h"


class Engine3D
{
public:
	Engine3D(int32_t openGlMajorVersion = 3, int32_t openGlMinorVersion = 3, int32_t width = 800, int32_t height = 600, const char* windowTitle = "Engine3D", bool resizable = true);
	~Engine3D();

	void PushMesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	void PushShader(std::string filepath);

	void Run();

	void SetWindowShouldClose();

private:
	const int32_t m_cOpenGlMajorVersion;
	const int32_t m_cOpenGlMinorVersion;
	int32_t m_Width;
	int32_t m_Height;
	int32_t m_BufferWidth;
	int32_t m_BufferHeight;
	bool m_Resizable;

	GLFWwindow* m_Window;

	Renderer m_Renderer;
	std::vector<std::shared_ptr<Shader>> m_Shaders;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;

private:
	void InitGlfw();
	void InitWindow(const char* windowTitle, bool resizable);
	void InitGlew();
	void InitOpenGLOptions();
};

