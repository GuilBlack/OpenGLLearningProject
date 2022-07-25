#include "Mesh.h"

#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

Mesh::Mesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout, 
	glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	: m_Vertices(vertices), m_Indices(indices), m_Layout(layout), 
		m_Position(position), m_Scale(scale), m_Rotation(rotation)
{
	m_Ib.AddData(indices, nbOfIndices);
	m_Vb.AddData(vertices, sizeOfVb);

	m_Va.AddVertexBuffer(m_Vb, m_Layout);
	m_Va.AddIndexBuffer(m_Ib);
	UpdateModelMatrix();
}

Mesh::~Mesh()
{}

void Mesh::Draw(Shader& shader)
{
	UpdateModelMatrix();
	UpdateUniforms(shader);

	shader.Bind();
	m_Va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, m_Va.GetIbCount(), GL_UNSIGNED_INT, 0));
}

void Mesh::Draw(std::shared_ptr<Shader> shader)
{
	UpdateModelMatrix();
	UpdateUniforms(shader);

	shader->Bind();
	m_Va.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, m_Va.GetIbCount(), GL_UNSIGNED_INT, 0));
}

void Mesh::MoveTo(glm::vec3 position)
{
	m_Position += position;
}

void Mesh::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void Mesh::Rotate(glm::vec3 rotation)
{
	m_Rotation += rotation;

	m_Rotation.x -= (m_Rotation.x >= 360.0f) * 360.0f;
	m_Rotation.y -= (m_Rotation.x >= 360.0f) * 360.0f;
	m_Rotation.z -= (m_Rotation.x >= 360.0f) * 360.0f;

}

void Mesh::SetRotation(glm::vec3 rotation)
{
	m_Rotation = rotation;
}

void Mesh::ScaleDown(glm::vec3 scale)
{
	m_Scale -= scale;
}

void Mesh::ScaleUp(glm::vec3 scale)
{
	m_Scale += scale;
}

void Mesh::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

void Mesh::UpdateModelMatrix()
{
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
}

/// <summary>
/// It will update all of the uniforms that we want to update
/// in a given shader.
/// </summary>
/// <param name="shader">: shader that we want to use to render this mesh.</param>
void Mesh::UpdateUniforms(Shader& shader) const
{
	shader.SetUniformMatrix4fv("model", m_ModelMatrix);
}

void Mesh::UpdateUniforms(std::shared_ptr<Shader> shader) const
{
	shader->SetUniformMatrix4fv("model", m_ModelMatrix);
}
