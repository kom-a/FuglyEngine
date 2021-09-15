#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <stdarg.h>

namespace Fugly
{
	class Log
	{
	private:
		Log();
		

	public:
		static bool Init();
		inline static std::shared_ptr <spdlog::logger> GetLogger() { return m_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> m_Logger;
	};
}

#define LOG_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) Log::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Log::GetLogger()->critical(__VA_ARGS__)

#define LOG_GL_ERRORS() {int OpenGLError = glGetError();\
								if(OpenGLError)\
									Log::GetLogger()->error("An OpenGL error occured: {0}", OpenGLError);}