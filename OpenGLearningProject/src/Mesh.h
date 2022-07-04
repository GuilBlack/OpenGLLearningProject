#pragma once

#include "VertexBufferLayout.h"

class Mesh
{
private:
	const void* m_Vertices;
	const void* m_Indices;

	VertexArray m_Va;
	IndexBuffer m_Ib;
	VertexBuffer m_Vb;
	VertexBufferLayout m_Layout;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	glm::mat4 m_ModelMatrix;
public:
	Mesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	~Mesh();

	void Draw(Shader& shader);

	void MoveTo(glm::vec3 position);
	void SetPosition(glm::vec3 position);

	void Rotate(glm::vec3 rotation);
	void SetRotation(glm::vec3 rotation);

	void ScaleDown(glm::vec3 scale);
	void ScaleUp(glm::vec3 scale);
	void SetScale(glm::vec3 scale);

private:
	void UpdateModelMatrix();
	void UpdateUniforms(Shader& shader) const;
};

