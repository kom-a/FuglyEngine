#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <GL/glew.h>

namespace Fugly
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, float v1, float v2);
		void SetUniform3f(const std::string& name, float v1, float v2, float v3);
		void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);

		void SetUniform2f(const std::string& name, const glm::vec2& vec);
		void SetUniform3f(const std::string& name, const glm::vec3& vec);
		void SetUniform4f(const std::string& name, const glm::vec4& vec);

		void SetMatrix3(const std::string& name, const glm::mat3& matrix);
		void SetMatrix4(const std::string& name, const glm::mat4& matrix);

	private:
		int GetUniformLocation(const std::string& name);
		GLuint CompileShader(const std::string& path, GLenum type);
		void CheckError(GLuint object, GLenum status);

	private:
		GLuint m_Shader;
		std::unordered_map<const char*, int>  m_LocationsCache;
	};
}

