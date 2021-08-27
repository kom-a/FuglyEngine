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
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);
	}

	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &m_VBO);
	}

	void Renderer::Begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_IndexBuffer = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
	}

	void Renderer::End()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	void Renderer::Submit(const Model& model)
	{
		for (const Mesh& mesh : model.GetMeshes())
		{
			for (const Vertex& vertex : mesh.GetVertices())
			{
				m_Buffer[m_VerticesCount].position = vertex.position;
				m_Buffer[m_VerticesCount].normal = vertex.normal;
				m_VerticesCount++;
			}

			const std::vector<unsigned int>& meshIndices = mesh.GetIndices();
			size_t meshIndicesSize = meshIndices.size();
			for (size_t i = 0; i < meshIndicesSize; i++)
			{
				m_IndexBuffer[m_IndicesCount] = meshIndices[i] + m_IndexOffset;
				m_IndicesCount++;
			}

			m_IndexOffset += mesh.GetVertices().size();
		}
	}

	void Renderer::Flush()
	{
		m_VertexArray.Bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0);

		/*m_VerticesCount = 0;
		m_IndicesCount = 0;
		m_IndexOffset = 0;*/
	}
}


