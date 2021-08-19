#pragma once

#define MAX_INDICES 1 << 16

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
		unsigned short* m_Buffer;
	};
}