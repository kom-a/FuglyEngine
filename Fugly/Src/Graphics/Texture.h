#pragma once

#include <string>

namespace Fugly
{
	class Texture
	{
	public:
		Texture(const std::string& filename, unsigned int unit);
		~Texture();

		void Bind(unsigned int unit);
		void Unbind();

	private:
		unsigned m_TexutreID;
	};
}