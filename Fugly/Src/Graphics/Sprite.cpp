#include "Sprite.h"

namespace Fugly
{
	Sprite::Sprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
		:
		m_Position(position),
		m_Size(size),
		m_Color(color)
	{

	}

	Sprite::~Sprite()
	{

	}
}


