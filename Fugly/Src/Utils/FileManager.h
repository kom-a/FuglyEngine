#pragma once

#include <string>

namespace Fugly
{
	class FileManager
	{
	public:
		static std::string ReadFile(const char* filepath);
	};
}

