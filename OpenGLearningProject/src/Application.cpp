#pragma once
#include "pch.h"
#include "Engine.h"
#include "ImGui/ImGuiLayer.h"
#include "Layers/Layer.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"

#include "vendor/glm/gtc/matrix_transform.hpp"

class GameLayer : public Layer
{
public:
	GameLayer()
		: m_Camera(glm::perspective(45.0f, (GLfloat)100 / (GLfloat)100, 0.1f, 500.0f), 
			glm::vec3(0.f, 0.f, 3.f), new OrbitalCameraControl(0.005f, 0.005f, 0.1f))
	{}

	virtual void OnAttach() override 
	{
		std::vector<uint32_t> indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 2, 1
		};

		std::vector<float> vertices = {
			-1.0f, -1.0f, 0.0f, 0.1f, 0.1f, 0.4f, 1.0f,
			0.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.4f, 1.0f,
			1.0f, -1.0f, 0.0f, 0.1f, 0.1f, 0.4f, 1.0f,
			0.0f, 1.0f, 0.0f, 0.1f, 0.1f, 0.4f, 1.0f
		};

		VertexBufferLayout layout = VertexBufferLayout(
			{
				{ "pos", GL_FLOAT, 3 },
				{ "col", GL_FLOAT, 4 }
			});

		Engine& engine = Engine::GetEngine();

		//m_Mesh.reset(Mesh::Create(vertices, vertices.size() * sizeof(m_Vertices[0]),
		//	indices, indices.size(), layout,
		//	glm::vec3(0.0f, 0.0f, -3.0f),
		//	glm::vec3(0.4f, 0.4f, 0.4f)));
		m_Texture.reset(Texture2D::Create("res/textures/earth.jpg", true));

		m_Mesh.reset(Mesh::CreateUVSphere(1, 64, 64));

		m_Shader.reset(Shader::Create("res/shaders/Basic.shader"));
		auto[width, height] = Engine::GetEngine().GetWindowDimensions();

		m_Camera.SetProjectionMatrix(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);

		m_Shader->BindCommand();
		m_Shader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
	}
	virtual void OnDetach() override {}
	virtual void OnUpdate() override 
	{
		if (Engine::GetEngine().DidWindowDimensionsChange())
		{
			auto [width, height] = Engine::GetEngine().GetWindowDimensions();
			m_Camera.SetProjectionMatrix(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);
			m_Shader->BindCommand();
			m_Shader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
		}
		m_Camera.Update();
		m_Shader->SetUniformMatrix4fv("view", m_Camera.GetViewMatrix());
		//m_Mesh->Rotate({ 0.0f, 1.0f, 0.0f });
		m_Mesh->Draw(m_Shader, m_Texture);
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

	virtual void OnEvent(Event& ev) override {}

private:
	std::vector<uint32_t> m_Indices;
	std::vector<float> m_Vertices;
	std::unique_ptr<Mesh> m_Mesh;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture2D> m_Texture;
	
	Camera m_Camera;

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