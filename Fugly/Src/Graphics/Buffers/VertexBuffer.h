#pragma once

namespace Fugly
{
	class VertexBuffer
	{
	public:
		VertexBuffer(float* data, int count);
		~VertexBuffer();

		void Bind();
		void Unbind();

	private:
		unsigned m_VBO;
	};
}