#include "VertexArray.h"

#include <GL/glew.h>

namespace Fugly
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_VAO);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_VAO);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
}
