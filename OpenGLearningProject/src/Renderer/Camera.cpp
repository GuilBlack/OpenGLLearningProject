#include "pch.h"
#include "Camera.h"
#include "Inputs/Input.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
////// OrbitalCameraControl /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void OrbitalCameraControl::UpdateCamera(const Camera& cam, glm::vec3& camPos, float& pitch, float& yaw)
{
	if (Input::IsKeyPressed(KRightAlt))
	{
		// Creating mouse delta
		glm::vec2 mouse = { Input::GetMousePosX(), Input::GetMousePosY() };
		glm::vec2 posDelta = mouse - m_LastMousePos;
		m_LastMousePos = mouse;

		//Zoom
		if (Input::IsMouseButtonPressed(MButtonRight))
		{
			m_Distance += m_ZoomSpeed * (m_Distance / 2) * posDelta.x;
			if (m_Distance < 1.f)
				m_Distance = 1.f;
		}
		//Rotate
		else if (Input::IsMouseButtonPressed(MButtonLeft))
		{
			yaw += m_RotationSpeed * posDelta.x;
			pitch += m_RotationSpeed * posDelta.y;
			if (pitch > 90.f)
			{
				pitch = 90.f;
			}
			else if (pitch < -90.f)
			{
				pitch = -90.f;
			}
		}
		//Pan
		else if (Input::IsMouseButtonPressed(MButtonMiddle))
		{
			m_FocalPoint += -cam.GetRightVector() * m_MovementSpeed * posDelta.x;
			m_FocalPoint += -cam.GetUpVector() * m_MovementSpeed * posDelta.y;
		}
	}
	camPos = m_FocalPoint - cam.GetForwardVector() * m_Distance;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////// Camera ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

Camera::Camera(glm::mat4 projMatrix, glm::vec3 pos, CameraControl* camControl)
	: m_ViewMatrix(1.f), m_ProjMatrix(projMatrix),
	m_Pos(0.f), m_Yaw(0.f), m_Pitch(0.f)
{
	m_CamControl.reset(camControl);
}

void Camera::Update()
{
	m_CamControl->UpdateCamera(*this, m_Pos, m_Pitch, m_Yaw);
	m_ViewMatrix = glm::toMat4(glm::conjugate(GetQuatOrientation())) * glm::translate(glm::mat4(1.f), -m_Pos);
}

void Camera::SetProjectionMatrix(float fovy, float aspectRatio, float nearPlane, float farPlane)
{
	m_ProjMatrix = glm::perspective(fovy, aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::GetForwardVector() const
{
	//-1 because of the handedness (right-handed) of the coord system
	return glm::rotate(GetQuatOrientation(), {0.f, 0.f, -1.f});
}

glm::vec3 Camera::GetUpVector() const
{
	return glm::rotate(GetQuatOrientation(), {0.f, 1.f, 0.f});
}

glm::vec3 Camera::GetRightVector() const
{
	return glm::rotate(GetQuatOrientation(), {1.f, 0.f, 0.f});
}

glm::quat Camera::GetQuatOrientation() const
{
	return glm::quat({ -glm::radians(m_Pitch), -glm::radians(m_Yaw), 0.f });
}