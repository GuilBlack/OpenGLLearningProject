#pragma once
#include "pch.h"
#include "Engine.h"
#include "ImGui/ImGuiLayer.h"
#include "Layers/Layer.h"
#include "vendor/glm/gtc/matrix_transform.hpp"

class GameLayer : public Layer
{
	virtual void OnAttach() override 
	{
		m_Indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 2, 1
		};

		m_Vertices = {
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

		m_Mesh.reset(Mesh::Create(m_Vertices.data(), m_Vertices.size() * sizeof(m_Vertices[0]),
			m_Indices.data(), m_Indices.size(), layout,
			glm::vec3(0.0f, 0.0f, -3.0f),
			glm::vec3(0.4f, 0.4f, 0.4f)));
		//engine.PushMesh(m_Vertices.data(), m_Vertices.size() * sizeof(m_Vertices[0]),
		//	m_Indices.data(), m_Indices.size(), layout,
		//	glm::vec3(0.0f, 0.0f, -3.0f),
		//	glm::vec3(0.4f, 0.4f, 0.4f));

		m_Shader.reset(Shader::Create("res/shaders/Basic.shader"));
		//engine.PushShader("res/shaders/Basic.shader");
		auto[width, height] = Engine::GetEngine().GetWindowDimensions();
		m_Projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);

		m_Shader->BindCommand();
		m_Shader->SetUniformMatrix4fv("projection", m_Projection);
	}
	virtual void OnDetach() override {}
	virtual void OnUpdate() override 
	{
		if (Engine::GetEngine().DidWindowDimensionsChange())
		{
			auto [width, height] = Engine::GetEngine().GetWindowDimensions();
			glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);
			m_Shader->BindCommand();
			m_Shader->SetUniformMatrix4fv("projection", projection);
		}

		m_Mesh->Rotate({ 0.0f, 1.0f, 0.0f });
		m_Mesh->Draw(m_Shader);
	}
	virtual void OnImGuiRender() override {}
	virtual void OnEvent(Event& ev) override {}

private:
	std::vector<uint32_t> m_Indices;
	std::vector<float> m_Vertices;
	std::unique_ptr<Mesh> m_Mesh;
	std::shared_ptr<Shader> m_Shader;
	glm::mat4 m_Projection;
};

int main()
{

	Engine* customEngine = new Engine();
	GameLayer* gameLayer = new GameLayer();
	customEngine->PushLayer(gameLayer);
	
	customEngine->Run();
	delete customEngine;
}