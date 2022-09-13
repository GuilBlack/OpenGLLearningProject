#pragma once
#include "glm/glm.hpp"

#include "Renderer/Shader.h"

class Camera
{

public:
	Camera(glm::vec3 pos, glm::vec3 up);

	void GetViewMatrix();

private:
	glm::mat4 m_ViewMatrix;

	float m_MovementSpeed;
	float m_MaxMovementSpeed;
	float m_Sensitivity;

	glm::vec3 m_WorldUp;
	glm::vec3 m_Forward;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Pos;

	float m_Yaw;
	float m_Pitch;

private:
	void UpdateVectors();
	void UpdateUniforms(std::shared_ptr<Shader>);
};

