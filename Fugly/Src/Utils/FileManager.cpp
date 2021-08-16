#include "FileManager.h"

#include <assert.h>

#include "Log.h"

namespace Fugly
{
	std::string FileManager::ReadFile(const char* filepath)
	{
		FILE* file;
		if (fopen_s(&file, filepath, "r"))
		{
			LOG_ERROR("Failed to open \"{0}\"", filepath);
			return "";
		}

		fseek(file, 0, SEEK_END);
		size_t length = ftell(file);
		char* data = new char[length + 1];
		memset(data, 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, sizeof(char), length, file);
		fclose(file);

		std::string source(data);
		delete[] data;

		return source;
	}

}