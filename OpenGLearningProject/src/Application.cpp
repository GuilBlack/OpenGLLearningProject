#pragma once
#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Mesh.h"
#include "Engine3D.h"

int main()
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

	Engine3D* customEngine = new Engine3D();

	customEngine->PushMesh(vertices.data(), vertices.size() * sizeof(vertices[0]),
		indices.data(), indices.size(), VertexBufferLayout::pLayout(),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.4f, 0.4f, 0.4f));

	customEngine->PushShader("res/shaders/Basic.shader");
	
	customEngine->Run();
	delete customEngine;
}