#include "IndexBuffer.h"

#include <GL/glew.h>

namespace Fugly
{
	IndexBuffer::IndexBuffer()
		: m_SpritesCount(0)
	{
		glGenBuffers(1, &m_IBO);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_IBO);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::Begin()
	{
		Bind();
		m_Buffer = (unsigned short*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void IndexBuffer::End()
	{
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	void IndexBuffer::Add()
	{
		int i = m_SpritesCount * 6;
		int currentIndex = m_SpritesCount * 4;
		m_Buffer[i + 0] = currentIndex + 0;
		m_Buffer[i + 1] = currentIndex + 1;
		m_Buffer[i + 2] = currentIndex + 2;
		m_Buffer[i + 3] = currentIndex + 0;
		m_Buffer[i + 4] = currentIndex + 2;
		m_Buffer[i + 5] = currentIndex + 3;

		m_SpritesCount++;
	}

	void IndexBuffer::Clear()
	{
		m_SpritesCount = 0;
	}
}



