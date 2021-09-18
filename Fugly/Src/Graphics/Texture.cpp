#include "Texture.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../Utils/Log.h"

namespace Fugly
{
	Texture::Texture(int width, int height, unsigned int unit)
		: m_Path(std::string())
	{
		glGenTextures(1, &m_TexutreID);
		Bind(unit);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Unbind();
	}

	Texture::Texture(const std::string& filename, unsigned int unit)
	{
		glGenTextures(1, &m_TexutreID);
		Bind(unit); 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
		int format = nrChannels == 4 ? GL_RGBA : GL_RGB;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			// glGenerateMipmap(GL_TEXTURE_2D);

			LOG_DEBUG("Texture loaded \"{0}\"", filename);
		}
		else
		{
			LOG_ERROR("Failed to load texture \"{0}\"", filename);
		}
		stbi_image_free(data);
		Unbind();

		m_Path = filename;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TexutreID);
	}

	void Texture::Bind(unsigned int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_TexutreID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}


