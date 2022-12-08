#pragma once
#include "pch.h"
#include "Engine.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"

#include "vendor/glm/gtc/matrix_transform.hpp"

class GameLayer : public Layer
{
public:
	GameLayer()
	{}

	virtual void OnAttach() override 
	{

		VertexBufferLayout layout = VertexBufferLayout(
			{
				{ "pos", GL_FLOAT, 3 },
				{ "col", GL_FLOAT, 4 }
			});

		Engine& engine = Engine::GetEngine();

		Renderer::GetRenderer().SetClearColor({ 0.10f, 0.12f, 0.10f, 1.f });
		
		m_Texture.reset(Texture2D::Create("res/textures/2k_earth.jpg"));

		m_Mesh.reset(Mesh::CreateUVSphere(1, 64, 64));

		m_Shader.reset(Shader::Create("res/shaders/Basic.glsl"));
		auto[width, height] = Engine::GetEngine().GetWindowDimensions();

		m_Camera.reset(new Camera(glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f),
			glm::vec3(0.f, 0.f, 3.f), new OrbitalCameraControl(0.005f, 0.005f, 0.1f)));
	}
	virtual void OnDetach() override {}
	virtual void OnUpdate(Timestep deltaTime) override 
	{
		Renderer::BeginScene(m_Camera);

		Renderer::Submit(m_Mesh, m_Shader, m_Texture);

		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override 
	{
		Engine& engine = Engine::GetEngine();
		auto[width, height] = engine.GetWindowDimensions();
		auto [posX, posY] = engine.GetWindowPos();
		ImGui::SetNextWindowSize(ImVec2((float)width / 4.5f, (float)height), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Once);
		if (!ImGui::Begin("Property editor"))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
		{
			ImGui::PushID(&m_Mesh);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			bool nodeOpen = ImGui::TreeNode("Mesh", "%s_%u", "Mesh", &m_Mesh);

			if (nodeOpen)
			{
				PushAttributeToImGui("Position", m_Mesh->m_Position, 0.01f);
				PushAttributeToImGui("Rotation", m_Mesh->m_Rotation, 1.f);
				PushAttributeToImGui("Scale", m_Mesh->m_Scale, 0.01f);
				ImGui::TreePop();
			}

			ImGui::PopID();
			ImGui::EndTable();
		}
		ImGui::End();
	}

	virtual void OnEvent(Event& ev) override 
	{
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WindowResizedEvent>(BIND_EVENT_FN(GameLayer::OnWindowResizedEvent));
	}

	bool OnWindowResizedEvent(WindowResizedEvent& ev)
	{
		auto [width, height] = ev.GetWidthAndHeight();
		m_Camera->SetProjectionMatrix(glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f));
		return false;
	};

private:
	std::vector<uint32_t> m_Indices;
	std::vector<float> m_Vertices;
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture2D> m_Texture;

	std::shared_ptr<Camera> m_Camera;

private:
		void PushAttributeToImGui(const char* attrName, glm::vec3& attr, float step)
		{
			ImGui::PushID(&attr);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();

			bool nodeOpen = ImGui::TreeNode("Attribute", "%s", attrName);

			if (nodeOpen)
			{
				PushFloatFieldToImGui("x", attr.x, step);
				PushFloatFieldToImGui("y", attr.y, step);
				PushFloatFieldToImGui("z", attr.z, step);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		void PushFloatFieldToImGui(const char* valName, float& val, float step)
		{
			ImGui::PushID(&val);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
			ImGui::TreeNodeEx("Field", flags, "%s", valName);

			ImGui::TableSetColumnIndex(1);
			ImGui::SetNextItemWidth(-FLT_MIN);

			ImGui::DragFloat("##value", &val, step);

			ImGui::NextColumn();

			ImGui::PopID();
		}
};

int main()
{

	Engine* customEngine = new Engine();
	GameLayer* gameLayer = new GameLayer();
	customEngine->PushLayer(gameLayer);
	
	customEngine->Run();
	delete customEngine;
}