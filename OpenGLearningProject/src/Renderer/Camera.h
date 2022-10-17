#pragma once
#include "glm/glm.hpp"

#include "Renderer/Shader.h"

class CameraControl
{
	friend class Camera;
protected:
	CameraControl() {}

private:
	/// <summary>
	/// Takes in the camera, the camera position, the pitch and the yaw and will calculate its new position and orientation with it.
	/// </summary>
	/// <param name="cam">the camera obj that uses</param>
	/// <param name="camPos">position of the cam to be updated</param>
	/// <param name="pitch">pitch of the cam to be updated</param>
	/// <param name="yaw">yaw of the cam to be updated</param>
	virtual void UpdateCamera(const Camera& cam, glm::vec3& camPos, float& pitch, float& yaw) {};
};

class OrbitalCameraControl : public CameraControl
{
public:
	OrbitalCameraControl(float movementSpeed, float zoomSpeed, float rotationSpeed)
		: m_MovementSpeed(movementSpeed), m_ZoomSpeed(zoomSpeed), m_RotationSpeed(rotationSpeed),
		m_LastMousePos(0.f), m_FocalPoint(0.f), m_Distance(20.f)
	{}

private:
	float m_MovementSpeed;
	float m_ZoomSpeed;
	float m_RotationSpeed;

	glm::vec2 m_LastMousePos;

	glm::vec3 m_FocalPoint;
	float m_Distance;

private:
	virtual void UpdateCamera(const Camera& cam, glm::vec3& camPos, float& pitch, float& yaw) override;
};

class Camera
{
public:
	Camera(glm::mat4 projMatrix, glm::vec3 pos, CameraControl* camControl);

	void Update();

	const glm::mat4& GetProjectionMatrix() const { return m_ProjMatrix; }
	void SetProjectionMatrix(float fovy, float aspectRatio, float nearPlane, float farPlane);

	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	glm::vec3 GetForwardVector() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetRightVector() const;

	const glm::vec3& GetPosition() const { return m_Pos; };
private:
	glm::quat GetQuatOrientation() const;

private:
	glm::mat4 m_ProjMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_Pos;

	float m_Yaw;
	float m_Pitch;

	std::unique_ptr<CameraControl> m_CamControl;
};