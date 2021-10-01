#pragma once

#include "Model.h"

#include "Buffers/Buffers.h"

#define MAX_RENDER_VERTICES 500000
#define MAX_RENDER_INDICES	5000 
#define MAX_RENDER_TEXTURES 16

namespace Fugly
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Begin();

		void Submit(const Model& model);
		void Flush();

	private:
		VertexArray m_VertexArray;
		unsigned m_VBO;
		unsigned m_IBO;

		Vertex* m_Buffer;
		Vertex* m_VertexPtr; // Pointer to current vertex in m_Buffer
		unsigned* m_IndexBuffer;

		size_t m_VerticesCount, m_IndicesCount;
		size_t m_IndexOffset;
	};
}