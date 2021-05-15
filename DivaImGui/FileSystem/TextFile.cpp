#include "TextFile.h"
#include <filesystem>

#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS


namespace fs = std::filesystem;

namespace ReplNau::FileSystem
{
	TextFile::TextFile(const std::string &path)
	{
		FileName = path;
	}

	TextFile::TextFile(const std::string &directory, const std::string &file)
	{
		auto fullPath = directory + "/" + file;
		FileName = fullPath;
	}

	TextFile::~TextFile()
	{
	}

	bool TextFile::OpenRead()
	{
		fs::path configPath = fs::path(FileName);

		if (!fs::exists(configPath))
			return false;

		std::ifstream fileStream(configPath.wstring().c_str());
		
		if (!fileStream.is_open())
			return false;

		Parse(fileStream);

		fileStream.close();

		return true;
	}
}
