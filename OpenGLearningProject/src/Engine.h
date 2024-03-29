#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Timestep.h"

#include "Renderer/Renderer.h"
#include "Entity/Mesh.h"
#include "Events/Event.h"
#include "Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

class WindowCloseEvent;

/// <summary>
/// basically where the application will run.
/// </summary>
class Engine
{
public:
	Engine(int32_t openGlMajorVersion = 4, int32_t openGlMinorVersion = 5, int32_t width = 1280, int32_t height = 720, const char* windowTitle = "Engine", bool resizable = true);
	~Engine();
	inline static Engine& GetEngine() { return *s_Engine; }
	inline static Timestep GetDeltaTime() { return GetEngine().m_DeltaTime; }
	inline static double GetCurrentTime() { return GetEngine().m_CurrTime; }

public:
	inline GLFWwindow& GetWindow() { return *m_Window; }
	std::pair<int32_t, int32_t> GetWindowDimensions() { return { m_WindowData.Width, m_WindowData.Height }; }
	std::pair<int32_t, int32_t> GetWindowPos() { return { m_WindowData.PosX, m_WindowData.PosY}; }
	inline bool DidWindowDimensionsChange()
	{
		return m_WindowData.DidWindowDimensionsChange;
	}

	void Run();

	void OnEvent(Event& ev);

	void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
	void PushOverlay(Layer* overlay) { m_LayerStack.PushLayer(overlay); }

	void PushMesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	void PushShader(std::string filepath);

	bool SetWindowShouldClose(WindowCloseEvent& e);

private:
	static Engine* s_Engine;
	bool m_Running = true;
	const int32_t m_cOpenGlMajorVersion;
	const int32_t m_cOpenGlMinorVersion;

	struct WindowData
	{
		std::string Title;
		int32_t Width, Height, TempWidth, TempHeight;
		int32_t PosX, PosY;
		bool DidWindowDimensionsChange;

		EventCallbackFn EventCallback;
		
		WindowData(std::string title, int32_t width, int32_t height, int32_t tempWidth, int32_t tempHeight)
			: Title(title), Width(width), Height(height), TempWidth(tempWidth), TempHeight(tempHeight), 
			PosX(0), PosY(0), DidWindowDimensionsChange(false) {}
	};
	
	bool m_Resizable;
	WindowData m_WindowData;
	GLFWwindow* m_Window;

	LayerStack m_LayerStack;
	ImGuiLayer* m_ImGuiLayer;
	std::vector<std::shared_ptr<Shader>> m_Shaders;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;

	Timestep m_DeltaTime{ 0.0 };
	double m_CurrTime = 0.0;
	double m_PrevTime = 0.0;

private:
	void InitGLFW();
	void InitWindow(const char* windowTitle, bool resizable);
	void InitCallbacks();
	inline void SetEventCallback(const EventCallbackFn& callback) { m_WindowData.EventCallback = callback; }
	void InitGlew();
	void InitOpenGLOptions();
};

