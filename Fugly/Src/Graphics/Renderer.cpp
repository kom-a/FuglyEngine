#include "Renderer.h"

#include <GL/glew.h>

#include "../Utils/Log.h"

namespace Fugly
{
	Renderer::Renderer()
		: m_VerticesCount(0),
		m_IndicesCount(0),
		m_IndexOffset(0),
		m_IndexBuffer(nullptr),
		m_Buffer(nullptr)
	{
		m_VertexArray.Bind();
		
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_RENDER_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, texCoords)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, color)));
		glEnableVertexAttribArray(3);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_RENDER_INDICES * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		m_Buffer = new Vertex[MAX_RENDER_VERTICES];
		m_VertexPtr = m_Buffer;
	}

	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
		delete m_Buffer;
	}

	void Renderer::Begin()
	{
		m_VerticesCount = 0;
		m_VertexPtr = m_Buffer;
	}

	void Renderer::Submit(const Model& model)
	{
		for (const Mesh* mesh : model.GetMeshes())
		{
			auto& vertices = mesh->GetVertices();
			if (m_VerticesCount + vertices.size() >= MAX_RENDER_VERTICES)
			{
				Flush();
				Begin();
			}
			
			memcpy(m_VertexPtr, &vertices[0], sizeof(Vertex) * vertices.size());
		}
	}

	void Renderer::Flush()
	{
		m_VertexArray.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VerticesCount * sizeof(Vertex), m_Buffer);

		glDrawArrays(GL_TRIANGLES, 0, m_VerticesCount);
	}
}


