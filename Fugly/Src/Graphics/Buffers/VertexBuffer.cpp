#include "VertexBuffer.h"

#include <GL/glew.h>

namespace Fugly
{
	VertexBuffer::VertexBuffer(size_t size)
	{
		glGenBuffers(1, &m_VBO);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteVertexArrays(1, &m_VBO);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Begin()
	{
		Bind();
		m_Buffer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void VertexBuffer::End()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

}


