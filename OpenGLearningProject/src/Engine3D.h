#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Mesh.h"
#include "Events/Event.h"

class WindowCloseEvent;

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

	void OnEvent(Event& e);
	bool SetWindowShouldClose(WindowCloseEvent& e);

private:
	bool m_Running = true;
	const int32_t m_cOpenGlMajorVersion;
	const int32_t m_cOpenGlMinorVersion;

	struct WindowData
	{
		std::string Title;
		int32_t Width, Height, TempWidth, TempHeight;

		EventCallbackFn EventCallback;
		
		WindowData(std::string title, int32_t width, int32_t height, int32_t tempWidth, int32_t tempHeight)
			: Title(title), Width(width), Height(height), TempWidth(tempWidth), TempHeight(tempHeight) {}
	};
	
	bool m_Resizable;
	WindowData m_WindowData;
	GLFWwindow* m_Window;

	Renderer m_Renderer;
	std::vector<std::shared_ptr<Shader>> m_Shaders;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;

private:
	void InitGLFW();
	void InitWindow(const char* windowTitle, bool resizable);
	void InitCallbacks();
	inline void SetEventCallback(const EventCallbackFn& callback) { m_WindowData.EventCallback = callback; }
	void InitGlew();
	void InitOpenGLOptions();
};

