#pragma once

#include "Renderer/VertexBufferLayout.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

class Mesh
{
public:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

public:
	Mesh() {};
	Mesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	Mesh(const std::vector<float>& vertices, uint32_t sizeOfVb, const std::vector<uint32_t>& indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	~Mesh();

	static Mesh* Create(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));
	static Mesh* Create(const std::vector<float>& vertices, uint32_t sizeOfVb, const std::vector<uint32_t>& indices, uint32_t nbOfIndices, VertexBufferLayout layout,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f),
		glm::vec3 rotation = glm::vec3(0.0f));

	void MoveTo(glm::vec3 position);
	void SetPosition(glm::vec3 position);

	void Rotate(glm::vec3 rotation);
	void SetRotation(glm::vec3 rotation);

	void ScaleDown(glm::vec3 scale);
	void ScaleUp(glm::vec3 scale);
	void SetScale(glm::vec3 scale);

	inline uint32_t GetIndexCount() const { return m_Va.GetIndexBuffer()->GetCount(); }
	inline void Bind() const { m_Va.Bind(); }

public:
	void Draw(Shader& shader);
	void Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture2D> texture);

	void UpdateModelMatrix();
	void UpdateUniforms(Shader& shader) const;
	void UpdateUniforms(std::shared_ptr<Shader> shader);

private:
	VertexArray m_Va;
	VertexBufferLayout m_Layout;

	glm::mat4 m_ModelMatrix;


public:
	static Mesh* CreateUVSphere(float radius, uint32_t nLatitude, uint32_t nLongitude);
};

