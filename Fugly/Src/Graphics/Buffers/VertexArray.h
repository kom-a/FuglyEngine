#pragma once

namespace Fugly
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

	private:
		unsigned m_VAO;
	};
}