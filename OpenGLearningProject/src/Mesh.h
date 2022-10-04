#pragma once

#include "Renderer/VertexBufferLayout.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

class Mesh
{
public:
	Mesh() {};
	Mesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	~Mesh();

	static Mesh* Create(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));

	void Draw(Shader& shader);
	void Draw(std::shared_ptr<Shader> shader);

	void MoveTo(glm::vec3 position);
	void SetPosition(glm::vec3 position);

	void Rotate(glm::vec3 rotation);
	void SetRotation(glm::vec3 rotation);

	void ScaleDown(glm::vec3 scale);
	void ScaleUp(glm::vec3 scale);
	void SetScale(glm::vec3 scale);

private:
	VertexArray m_Va;
	VertexBufferLayout m_Layout;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	glm::mat4 m_ModelMatrix;

private:
	void UpdateModelMatrix();
	void UpdateUniforms(Shader& shader) const;
	void UpdateUniforms(std::shared_ptr<Shader> shader) const;
};

