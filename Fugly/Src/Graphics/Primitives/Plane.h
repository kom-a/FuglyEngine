#pragma once

#include "../Buffers/VertexArray.h"
#include "../Texture.h"

namespace Fugly
{
	class Plane
	{
	public:
		Plane();
		~Plane();

		void Render();

	private:
		VertexArray m_VAO;
		unsigned m_VBO;
		unsigned m_IBO;

		Texture m_Texture;
	};
}