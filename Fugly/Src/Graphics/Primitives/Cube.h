#pragma once

#include <glm/glm.hpp>
#include "../Buffers/Buffers.h"

#include "../Texture.h"

namespace Fugly
{

	class Cube
	{
	public:
		Cube(const glm::vec3& color);
		~Cube();

		void Render();

	private:
		glm::vec3 m_Color;

		VertexArray m_VAO;
		unsigned m_VBO;
		unsigned m_IBO;

		Texture m_Texture;
	};
}