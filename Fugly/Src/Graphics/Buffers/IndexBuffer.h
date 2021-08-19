#pragma once

#define INDECIES_BUFFER_SIZE 1 << 24

namespace Fugly
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		void Bind();
		void Unbind();

		void Begin();
		void End();

		void Add();
		void Clear();

		inline size_t GetCount() const { return m_SpritesCount * 6; }

	private:
		unsigned m_IBO;
		size_t m_SpritesCount;
		unsigned int* m_Buffer;
	};
}