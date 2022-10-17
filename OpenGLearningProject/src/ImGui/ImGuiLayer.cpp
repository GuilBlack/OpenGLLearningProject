#include "pch.h"
#include "ImGuiLayer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Inputs/InputCodes.h"

#include <../Engine.h>

ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") 
{}

ImGuiLayer::~ImGuiLayer()
{
	OnDetach();
}

void ImGuiLayer::OnAttach()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([]()
		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			// Setup Platform/Renderer backends
			Engine& engine =  Engine::GetEngine();
			ImGui_ImplGlfw_InitForOpenGL(&engine.GetWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 420");
		});
}

void ImGuiLayer::OnDetach()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([]()
		{
			ImGui_ImplGlfw_Shutdown();
			ImGui_ImplOpenGL3_Shutdown();
			ImGui::DestroyContext();
		});
}

void ImGuiLayer::Begin()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([]()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		});
}

void ImGuiLayer::End()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([]()
		{
			ImGuiIO io = ImGui::GetIO();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		});
}

void ImGuiLayer::OnImGuiRender()
{
	static bool showDemo = true;

	if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);
}

