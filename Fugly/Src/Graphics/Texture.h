#pragma once

#include <string>

namespace Fugly
{
	class Texture
	{
	public:
		Texture(int width, int height, unsigned int unit);
		Texture(const std::string& filename, unsigned int unit);
		~Texture();

		void Bind(unsigned int unit) const;
		void Unbind() const;

		inline unsigned GetID() const { return m_TexutreID; }
		inline std::string GetPath() const { return m_Path; }
		inline size_t GetSize() const { return m_Size; }

	private:
		unsigned m_TexutreID;
		std::string m_Path;
		size_t m_Size;
	};
}