#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Fugly
{

	class Camera
	{
	public:
		Camera(const glm::vec3& eye, const glm::vec3& front);
		~Camera();

		void Update(float deltaTime);

		inline glm::vec3 GetPosition() const { return m_Eye; }
		inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Eye, m_Eye + m_Front, glm::vec3(0, 1, 0)); }

	private:
		void UpdatePosition(float deltaTime);
		void UpdateAngles(float deltaTime);

	private:
		glm::vec3 m_Eye;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		
		float m_Pitch;
		float m_Yaw;
		bool m_FirstMouse;

		float m_Sensitivity;

		glm::vec2 m_LastMouse;
	};
}