#include "Camera.h"

#include "../Input/Input.h"

namespace Fugly
{
	Camera::Camera(const glm::vec3& eye, const glm::vec3& front)
		: m_Eye(eye),
		m_Front(front),
		m_Up(glm::vec3(0, 1, 0)),
		m_Pitch(0.0f),
		m_Yaw(-90.0f),
		m_FirstMouse(true),
		m_Sensitivity(0.1f),
		m_LastMouse(glm::vec2(0))
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::Update(float deltaTime)
	{
		UpdatePosition(deltaTime);
		UpdateAngles(deltaTime);
	}

	void Camera::UpdatePosition(float deltaTime)
	{
		float cameraSpeed = 7.5f * deltaTime;

		if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			cameraSpeed *= 3;

		if (Keyboard::IsKeyPressed(GLFW_KEY_W))
			m_Eye += m_Front * cameraSpeed;
		if (Keyboard::IsKeyPressed(GLFW_KEY_S))
			m_Eye -= m_Front * cameraSpeed;
		if (Keyboard::IsKeyPressed(GLFW_KEY_D))
			m_Eye += glm::normalize(glm::cross(m_Front, m_Up)) * cameraSpeed;
		if (Keyboard::IsKeyPressed(GLFW_KEY_A))
			m_Eye -= glm::normalize(glm::cross(m_Front, m_Up)) * cameraSpeed;
		if (Keyboard::IsKeyPressed(GLFW_KEY_SPACE))
			m_Eye += glm::vec3(0, 1, 0) * cameraSpeed;
		if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
			m_Eye -= glm::vec3(0, 1, 0) * cameraSpeed;
	}

	void Camera::UpdateAngles(float deltaTime)
	{
		glm::vec2 mouse = glm::vec2(Mouse::GetX(), Mouse::GetY());

		glm::vec2 offset = mouse - m_LastMouse;
		offset.y *= -1;

		m_LastMouse = mouse;

		offset *= m_Sensitivity;

		m_Yaw += offset.x;
		m_Pitch += offset.y;

		m_Pitch = glm::clamp(m_Pitch, -89.99f, 89.99f);

		m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front.y = sin(glm::radians(m_Pitch));
		m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Front = glm::normalize(m_Front);
		
		glm::vec3 right = glm::normalize(glm::cross(m_Front, glm::vec3(0, 1, 0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(right, m_Front));
	}

}
