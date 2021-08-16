#include "Log.h"

namespace Fugly
{
	std::shared_ptr<spdlog::logger> Log::m_Logger;

	Log::Log()
	{
		
	}

	bool Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		spdlog::set_level(spdlog::level::level_enum::trace);
		m_Logger = spdlog::stderr_color_mt("Fugly");

		return m_Logger != nullptr;
	}
}