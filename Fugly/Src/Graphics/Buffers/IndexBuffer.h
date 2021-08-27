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

		void Add(unsigned int index);
		void Clear();

		inline size_t GetCount() const { return m_Count; }

	private:
		unsigned m_IBO;
		size_t m_Count;
		unsigned int* m_Buffer;
	};
}