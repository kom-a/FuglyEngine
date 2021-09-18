#include "Plane.h"

#include <GL/glew.h>

namespace Fugly
{
	Plane::Plane()
		: m_Texture(Texture("Res/Plane.png", 0))
	{
		m_VAO.Bind();

		// position3 normal3 texCoords2 color3
		float vertices[] = {
			-0.5f, -0.5f, 0.5f,		0.0f,  1.0f, 0.0f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.5f,		0.0f,  1.0f, 0.0f,		25.0f, 0.0f,		1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f,  1.0f, 0.0f,		0.0f, 25.0f,		1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	0.0f,  1.0f, 0.0f,		25.0f, 25.0f,		1.0f, 1.0f, 1.0f,
		};

		unsigned int indices[] = {
			0, 1, 2, 1, 3, 2
		};

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (const void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (const void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (const void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (const void*)(8 * sizeof(float)));
	}

	Plane::~Plane()
	{

	}

	void Plane::Render()
	{
		m_VAO.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_Texture.Bind(0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}