#include "Shader.h"

#include "../Utils/FileManager.h"
#include "../Utils/Log.h"

namespace Fugly
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		m_Shader = glCreateProgram();
		GLuint vertex = CompileShader(vertexPath, GL_VERTEX_SHADER);
		GLuint fragment = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);
		glAttachShader(m_Shader, vertex);
		glAttachShader(m_Shader, fragment);
		glLinkProgram(m_Shader);
		CheckError(m_Shader, GL_LINK_STATUS);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Shader);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_Shader);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	GLuint Shader::CompileShader(const std::string& path, GLenum type)
	{
		GLuint shader = glCreateShader(type);

		std::string source = FileManager::ReadFile(path.c_str());
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, 0);
		glCompileShader(shader);
		CheckError(shader, GL_COMPILE_STATUS);

		return shader;
	}

	void Shader::CheckError(GLuint object, GLenum status)
	{
		switch (status)
		{
		case GL_COMPILE_STATUS:
		{
			int success;
			glGetShaderiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetShaderInfoLog(object, length, &length, infoLog);
				LOG_ERROR(infoLog);
			}
		} break;
		case GL_LINK_STATUS:
		{
			int success;
			glGetProgramiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetProgramInfoLog(object, length, &length, infoLog);
				LOG_ERROR(infoLog);
			}
		} break;
		default:
		{
			LOG_ERROR("No such status.");
		}
		}
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_LocationsCache.find(name.c_str()) == m_LocationsCache.end())
		{
			int location = glGetUniformLocation(m_Shader, name.c_str());
			m_LocationsCache.insert(std::make_pair(name.c_str(), location));
			if (location == -1)
				LOG_ERROR("Could not find uniform \"{0}\".", name);
			return location;
		}

		return m_LocationsCache.at(name.c_str());
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform2f(const std::string& name, float v1, float v2)
	{
		glUniform2f(GetUniformLocation(name), v1, v2);
	}

	void Shader::SetUniform3f(const std::string& name, float v1, float v2, float v3)
	{
		glUniform3f(GetUniformLocation(name), v1, v2, v3);
	}

	void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
	{
		glUniform4f(GetUniformLocation(name), v1, v2, v3, v4);
	}

	void Shader::SetUniform2f(const std::string& name, const glm::vec2& vec)
	{
		SetUniform2f(name, vec.x, vec.y);
	}

	void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec)
	{
		SetUniform3f(name, vec.x, vec.y, vec.z);
	}

	void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec)
	{
		SetUniform4f(name, vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::SetMatrix4(const std::string&  name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}
}
