#include "Renderer.h"

#include <GL/glew.h>

#include "../Utils/Log.h"

namespace Fugly
{
	Renderer::Renderer()
		: m_SpritesCount(0)
	{
		m_VertexArray.Bind();
		
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererLayout), (const void*)offsetof(RendererLayout, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RendererLayout), (const void*)offsetof(RendererLayout, color));
		glEnableVertexAttribArray(1);
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::Begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (RendererLayout*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void Renderer::End()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void Renderer::Submit(const Sprite& sprite)
	{
		size_t i = m_SpritesCount * 6;
		m_Buffer[i + 0].position = sprite.GetPosition();
		m_Buffer[i + 0].color = sprite.GetColor();

		m_Buffer[i + 1].position = sprite.GetPosition() + glm::vec3(sprite.GetSize().x, 0, 0);
		m_Buffer[i + 1].color = sprite.GetColor();

		m_Buffer[i + 2].position = sprite.GetPosition() + sprite.GetSize();
		m_Buffer[i + 2].color = sprite.GetColor();

		m_Buffer[i + 3].position = sprite.GetPosition();
		m_Buffer[i + 3].color = sprite.GetColor();

		m_Buffer[i + 4].position = sprite.GetPosition() + sprite.GetSize();
		m_Buffer[i + 4].color = sprite.GetColor();

		m_Buffer[i + 5].position = sprite.GetPosition() + glm::vec3(0, sprite.GetSize().y, 0.0f);
		m_Buffer[i + 5].color = sprite.GetColor();

		m_SpritesCount++;
	}

	void Renderer::Flush()
	{
		m_VertexArray.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glDrawArrays(GL_TRIANGLES, 0, m_SpritesCount * 6);
		m_SpritesCount = 0;
	}
}


