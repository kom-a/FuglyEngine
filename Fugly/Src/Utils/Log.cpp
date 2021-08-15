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
		m_Logger = spdlog::stderr_color_mt("Fugly");

		return m_Logger != nullptr;
	}
}