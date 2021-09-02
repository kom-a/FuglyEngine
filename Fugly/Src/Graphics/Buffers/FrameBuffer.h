#pragma once

namespace Fugly
{
	class FrameBuffer
	{
	public:
		FrameBuffer(unsigned int textureID);
		~FrameBuffer();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_ID;
	};
}