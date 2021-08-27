#include "IndexBuffer.h"

#include <GL/glew.h>

#include "../../Utils/Log.h"

namespace Fugly
{
	IndexBuffer::IndexBuffer()
		: m_Count(0)
	{
		glGenBuffers(1, &m_IBO);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDECIES_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);
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
		m_Buffer = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void IndexBuffer::End()
	{
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	void IndexBuffer::Add(unsigned int index)
	{
		m_Buffer[m_Count++] = index;
	}

	void IndexBuffer::Clear()
	{
		m_Count = 0;
	}
}



