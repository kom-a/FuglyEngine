#pragma once

#include <cstdint>

namespace Fugly
{
	class FrameBuffer
	{
	public:
		FrameBuffer(int32_t width, int32_t height);
		~FrameBuffer();

		inline unsigned int GetColorBuffer() const { return m_TextureID; }

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_ID;
		unsigned int m_TextureID;
		unsigned int m_RBO;
	};
}