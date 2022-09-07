#pragma once
#include "pch.h"
#include "Engine.h"
#include "ImGui/ImGuiLayer.h"

int main()
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

	Engine* customEngine = new Engine();

	VertexBufferLayout layout = VertexBufferLayout(
		{
			{ "pos", GL_FLOAT, 3 },
			{ "col", GL_FLOAT, 4 }
		});

	customEngine->PushMesh(vertices.data(), vertices.size() * sizeof(vertices[0]),
		indices.data(), indices.size(), layout,
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.4f, 0.4f, 0.4f));

	customEngine->PushShader("res/shaders/Basic.shader");
	
	customEngine->Run();
	delete customEngine;
}