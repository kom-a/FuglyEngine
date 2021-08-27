#pragma once

#include "Model.h"

#include "Buffers/Buffers.h"

#define RENDERER_BUFFER_SIZE 1 << 24

namespace Fugly
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Begin();
		void End();

		void Submit(const Model& model);
		void Flush();

	private:
		VertexArray m_VertexArray;

		unsigned m_VBO;
		unsigned m_IBO;

		Vertex* m_Buffer;
		unsigned* m_IndexBuffer;

		size_t m_VerticesCount, m_IndicesCount;
		size_t m_IndexOffset;
	};
}