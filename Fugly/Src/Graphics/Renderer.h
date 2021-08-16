#pragma once

#include "Sprite.h"

#include "Buffers/Buffers.h"

#define RENDERER_BUFFER_SIZE 32768 * 256

namespace Fugly
{
	struct RendererLayout
	{
		glm::vec3 position;
		glm::vec4 color;
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Submit(const Sprite& sprite);
		void Flush();

	private:
		VertexArray m_VertexArray;
		unsigned m_VBO;

		size_t m_SpritesCount;
	};
}