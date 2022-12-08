#include "pch.h"
#include "Mesh.h"
#include "Renderer/Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

Mesh::Mesh(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout, 
	glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	: m_Layout(layout), m_Position(position), m_Scale(scale), m_Rotation(rotation)
{
	std::shared_ptr<VertexBuffer> vb;
	std::shared_ptr<IndexBuffer> ib;

	vb.reset(new VertexBuffer(vertices, sizeOfVb));
	ib.reset(new IndexBuffer(indices, nbOfIndices));

	vb->SetLayout(layout);

	m_Va.AddVertexBuffer(vb);
	m_Va.AddIndexBuffer(ib);
	UpdateModelMatrix();
}

Mesh::Mesh(const std::vector<float>& vertices, uint32_t sizeOfVb, const std::vector<uint32_t>& indices, uint32_t nbOfIndices, VertexBufferLayout layout, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	: m_Layout(layout), m_Position(position), m_Scale(scale), m_Rotation(rotation)
{
	std::shared_ptr<VertexBuffer> vb;
	std::shared_ptr<IndexBuffer> ib;

	vb.reset(new VertexBuffer(vertices, sizeOfVb));
	ib.reset(new IndexBuffer(indices, nbOfIndices));

	vb->SetLayout(layout);

	m_Va.AddVertexBuffer(vb);
	m_Va.AddIndexBuffer(ib);
	UpdateModelMatrix();
}

Mesh::~Mesh()
{}

Mesh* Mesh::Create(const void* vertices, uint32_t sizeOfVb, const void* indices, uint32_t nbOfIndices, VertexBufferLayout layout, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	return new Mesh(vertices, sizeOfVb, indices, nbOfIndices, layout, position, scale, rotation);
}

Mesh* Mesh::Create(const std::vector<float>& vertices, uint32_t sizeOfVb, const std::vector<uint32_t>& indices, uint32_t nbOfIndices, VertexBufferLayout layout, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	return new Mesh(vertices, sizeOfVb, indices, nbOfIndices, layout, position, scale, rotation);
}

void Mesh::Draw(Shader& shader)
{
	UpdateModelMatrix();
	UpdateUniforms(shader);

	shader.Bind();
	m_Va.Bind();
	glDrawElements(GL_TRIANGLES, m_Va.GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
}

void Mesh::Draw(std::shared_ptr<Shader> shader, std::shared_ptr<Texture2D> texture)
{
	UpdateModelMatrix();
	UpdateUniforms(shader);

	Renderer::GetRendererCommandQueue().PushCommand([this, shader, texture]()
		{
			shader->Bind();
			texture->Bind();
			m_Va.Bind();
			glDrawElements(GL_TRIANGLES, m_Va.GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
		});
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
	m_Rotation.y -= (m_Rotation.y >= 360.0f) * 360.0f;
	m_Rotation.z -= (m_Rotation.z >= 360.0f) * 360.0f;

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
	shader.SetUniformMatrix4fv("u_Model", m_ModelMatrix);
}

void Mesh::UpdateUniforms(std::shared_ptr<Shader> shader)
{
	UpdateModelMatrix();
	shader->SetUniformMatrix4fv("u_Model", m_ModelMatrix);
}

/// <summary>
/// Creates a UV Sphere
/// </summary>
/// <param name="nLatitude">Number of latitude stripes</param>
/// <param name="nLongitude">Number of longitude stripes</param>
/// <returns></returns>
Mesh* Mesh::CreateUVSphere(float radius, uint32_t nLatitude = 32, uint32_t nLongitude = 32)
{
	if (nLatitude < 1)
		nLatitude = 1;
	if (nLongitude < 3)
		nLongitude = 3;

	uint32_t nVertices = nLatitude * (nLongitude + 1) + (nLongitude * 2);
	//-1 to nLat because it wouldn't make sense otherwise.
	uint32_t nIndices = 2 * 3 * nLongitude + 2 * 3 * (nLatitude - 1) * nLongitude;

	VertexBufferLayout layout = VertexBufferLayout(
		{
			{ "pos",	GL_FLOAT, 3 },
			{ "uv" ,	GL_FLOAT, 2 },
			{ "normal",	GL_FLOAT, 3 }
			//{ "col", GL_FLOAT, 4 }
		});
	int stride = layout.GetStride() / sizeof(float);

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	vertices.resize(nVertices * stride);
	indices.resize(nIndices);

	// here, latitude points should be mapped between -90 and 90 degrees (or -PI/2 to PI/2).
	// +1 to nLat because it wouldn't make sense otherwise.
	float latitudeSlope = (float)M_PI / (float)(nLatitude + 1);
	// here, longitude points should be mapped between -180 and 180 degrees (or -PI to PI).
	float longitudeSlope = (2.f * (float)M_PI) / (float)nLongitude;

	uint32_t count = 0;
	// add north pole
	for (uint32_t i = 1; i <= nLongitude; ++i)
	{
		vertices[count * stride] = 0.0f; vertices[count * stride + 1] = radius; vertices[count * stride + 2] = 0.0f;
		vertices[count * stride + 3] = (float)i / ((float)nLongitude + 1.0f); vertices[count * stride + 4] = 0.0f;
		vertices[count * stride + 5] = 0.0f; vertices[count * stride + 6] = 1.0f; vertices[count * stride + 7] = 0.0f;
		//vertices[count * stride + 5] = 1.0f; vertices[count * stride + 6] = 1.0f;
		//vertices[count * stride + 7] = 1.0f; vertices[count * stride + 8] = 1.0f;
		++count;
	}

	//middle quads
	for (uint32_t i = 1; i < (nLatitude + 1); ++i)
	{
		float pLat = latitudeSlope * (float)i;
		for (uint32_t j = 0; j < nLongitude + 1; ++j)
		{
			float pLon = longitudeSlope * (float)j;
			glm::vec3 point = { sinf(pLat) * cosf(pLon), cosf(pLat), sinf(pLat) * sinf(pLon) };

			// coords
			vertices[count * stride] = radius * point.x;
			vertices[count * stride + 1] = radius * point.y;
			vertices[count * stride + 2] = -radius * point.z;

			// UVs
			vertices[count * stride + 3] = (float)j / (float)nLongitude;
			vertices[count * stride + 4] = (float)i / ((float)nLatitude + 1.0f);

			// normals
			vertices[count * stride + 5] = point.x;
			vertices[count * stride + 6] = point.y;
			vertices[count * stride + 7] = point.z;

			// colors
			//vertices[count * stride + 5] = 1.0f;
			//vertices[count * stride + 6] = 1.0f;
			//vertices[count * stride + 7] = 1.0f;
			//vertices[count * stride + 8] = 1.0f;
			++count;
		}
	}

	//add south pole
	for (uint32_t i = 1; i <= nLongitude; ++i)
	{
		vertices[count * stride] = 0.0f; vertices[count * stride + 1] = -radius; vertices[count * stride + 2] = 0.0f;
		vertices[count * stride + 3] = (float)i / ((float)nLongitude + 1.0f); vertices[count * stride + 4] = 1.0f;
		vertices[count * stride + 5] = 0.0f; vertices[count * stride + 6] = -1.0f; vertices[count * stride + 7] = 0.0f;
		
		//vertices[count * stride + 5] = 1.0f; vertices[count * stride + 6] = 1.0f;
		//vertices[count * stride + 7] = 1.0f; vertices[count * stride + 8] = 1.0f;
		++count;
	}

	count = 0;
	//north pole indices
	for (uint32_t i = 0; i < nLongitude; ++i)
	{
		indices[count++] = i;
		indices[count++] = (nLongitude - 1) + i + 2;
		indices[count++] = (nLongitude - 1) + i + 1;
	}

	//middle quads
	for (uint32_t i = 0; i < nLatitude - 1; ++i)
	{
		for (uint32_t j = 0; j < nLongitude; ++j)
		{
			uint32_t index[4] = {
				nLongitude + i * (nLongitude + 1) + j,
				nLongitude + i * (nLongitude + 1) + (j + 1),
				nLongitude + (i + 1) * (nLongitude + 1) + (j + 1),
				nLongitude + (i + 1) * (nLongitude + 1) + j
			};

			indices[count++] = index[0];
			indices[count++] = index[1];
			indices[count++] = index[2];

			indices[count++] = index[0];
			indices[count++] = index[2];
			indices[count++] = index[3];
		}
	}

	//south pole indices
	const uint32_t southPoleIndex = nVertices - nLongitude;
	for (uint32_t i = 0; i < nLongitude; ++i)
	{
		indices[count++] = southPoleIndex + i;
		indices[count++] = southPoleIndex - (nLongitude + 1) + i;
		indices[count++] = southPoleIndex - (nLongitude + 1) + i + 1;
	}

	return new Mesh(vertices, nVertices * stride * sizeof(vertices[0]), indices, nIndices, layout, glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f, 1.f, 1.f));
}
