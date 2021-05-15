#include "Lua/Lua.h"
#include "Hook/Hook.h"
#include "MainModule.h"
#include <filesystem>
#include "FileSystem/ConfigFile.h"

namespace ReplNau
{
	typedef std::filesystem::path fspath;

	std::string* MainModule::moduleDirectory;

	HWND MainModule::DivaWindowHandle;
	HMODULE MainModule::Module;

	void MainModule::initialize()
	{
		//const std::string RESOLUTION_CONFIG_FILE_NAME = "repl.ini";

		//ReplNau::FileSystem::ConfigFile resolutionConfig(MainModule::GetModuleDirectory(), RESOLUTION_CONFIG_FILE_NAME.c_str());
		//bool success = resolutionConfig.OpenRead();
		//if (!success)
		//{
		//	printf("[ReplNau] Unable to parse %s\n", RESOLUTION_CONFIG_FILE_NAME.c_str());
		//}

		printf("[ReplNau] Initialize\n");
		ReplNau::Hook::init();
		ReplNau::Lua::init();
	}

	std::string MainModule::GetModuleDirectory()
	{
		if (moduleDirectory == nullptr)
		{
			WCHAR modulePathBuffer[MAX_PATH];
			GetModuleFileNameW(MainModule::Module, modulePathBuffer, MAX_PATH);

			fspath configPath = fspath(modulePathBuffer).parent_path();
			moduleDirectory = new std::string(configPath.string());
		}

		return *moduleDirectory;
	}

	RECT MainModule::GetWindowBounds()
	{
		RECT windowRect;
		GetWindowRect(DivaWindowHandle, &windowRect);

		return windowRect;
	}
}