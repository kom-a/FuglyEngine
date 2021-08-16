#include "VertexBuffer.h"

#include <GL/glew.h>

namespace Fugly
{
	VertexBuffer::VertexBuffer(float* data, int count)
	{
		glGenBuffers(1, &m_VBO);
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
}


