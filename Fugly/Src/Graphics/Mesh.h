#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Fugly
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		void Render();

		inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		inline const std::vector<unsigned int>& GetIndices() const { return m_Indices; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
	};

}