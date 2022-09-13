#include "pch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 up)
	: m_Pos(pos), m_WorldUp(up)
{
	m_Forward = { 0.0f, 0.0f ,1.0f };
	m_MaxMovementSpeed = 3.0f;
}

void Camera::GetViewMatrix()
{
	UpdateVectors();
	m_ViewMatrix = glm::lookAt(m_Pos, m_Pos + m_Forward, m_WorldUp);
}

void Camera::UpdateVectors()
{
	m_Forward.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	m_Forward.y = sin(glm::radians(m_Pitch));
	m_Forward.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));

	m_Forward = glm::normalize(m_Forward);
	glm::vec3 upChanges = m_Forward * glm::dot(m_Forward, m_WorldUp);
	m_Up = glm::normalize(m_Up - upChanges);
	m_Right = glm::cross(m_Forward, m_Up);
}
