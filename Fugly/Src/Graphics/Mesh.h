#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Fugly
{
	class Mesh
	{
	public:
		Mesh(const std::vector<glm::vec3>& positions);
		~Mesh();

	private:
		std::vector<glm::vec3> m_Positions;
	};

}