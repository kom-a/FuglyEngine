#pragma once

namespace Fugly
{
	class VertexBuffer
	{
	public:
		VertexBuffer(size_t size);
		~VertexBuffer();

		void Bind();
		void Unbind();

		void Begin();
		void End();

	private:
		unsigned m_VBO;
		float* m_Buffer;
	};
}