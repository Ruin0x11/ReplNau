#pragma once
#include <Windows.h>
#include <string>

namespace ReplNau
{
	class MainModule
	{
	private:
		static std::string *moduleDirectory;

	public:
		static HWND DivaWindowHandle;
		static HMODULE Module;

		static std::string GetModuleDirectory();
		static RECT GetWindowBounds();

		static void initialize();
		static void deinitialize();

		static void InitializeTick();
		static void UpdateTick();
	};
}
