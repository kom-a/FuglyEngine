#pragma once

#include <glm/glm.hpp>

namespace Fugly
{
	class Sprite
	{
	public:
		Sprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		~Sprite();

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetSize() const { return m_Size; }
		inline const glm::vec4& GetColor() const { return m_Color; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Size;
		glm::vec4 m_Color;
	};
}