#include "Cube.h"

#include <GL/glew.h>

namespace Fugly
{
	Cube::Cube(const glm::vec3& color)
		: m_Color(color),
		m_Texture(Texture("Res/White.png", 0))
	{
		m_VAO.Bind();

		// normal component
		float n = 1.0f / std::sqrt(3.0f);

		// position3 normal3 texCoords2 color3
		float vertices[] = {
			-0.5f, -0.5f, 0.5f,		-n, -n,  n,		0.0f, 0.0f,		color.r, color.g, color.b,
			 0.5f, -0.5f, 0.5f,		 n, -n,  n,		0.0f, 0.0f,		color.r, color.g, color.b,
			 0.5f,  0.5f, 0.5f,		 n,  n,  n,		0.0f, 0.0f,		color.r, color.g, color.b,
			-0.5f,  0.5f, 0.5f,		-n,  n,  n,		0.0f, 0.0f,		color.r, color.g, color.b,

			-0.5f, -0.5f, -0.5f,	-n, -n, -n,		0.0f, 0.0f,		color.r, color.g, color.b,
			 0.5f, -0.5f, -0.5f,	 n, -n, -n,		0.0f, 0.0f,		color.r, color.g, color.b,
			 0.5f,  0.5f, -0.5f,	 n,  n, -n,		0.0f, 0.0f,		color.r, color.g, color.b,
			-0.5f,  0.5f, -0.5f,	-n,  n, -n,		0.0f, 0.0f,		color.r, color.g, color.b,
		};

		unsigned int indices[] = {
			// front
			0, 1, 2, 0, 2, 3,
			// back
			4, 5, 6, 4, 6, 7,
			// left
			0, 3, 4, 3, 4, 7,
			// right 
			1, 5, 6, 1, 6, 2,
			// up
			3, 2, 6, 6, 7, 3,
			// down
			0, 4, 5, 1, 0, 5,
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

	Cube::~Cube()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
	}

	void Cube::Render()
	{
		m_VAO.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_Texture.Bind(0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

}
