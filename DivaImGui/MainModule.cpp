#include "Lua/Lua.h"
#include "Hook/Hook.h"
#include "MainModule.h"
#include <filesystem>
#include "FileSystem/ConfigFile.h"
#include "Input/Mouse/Mouse.h"
#include "Input/Xinput/Xinput.h"
#include "Input/Keyboard/Keyboard.h"
#include "Input/DirectInput/DirectInput.h"
#include "Input/DirectInput/Ds4/DualShock4.h"
#include "Input/DirectInput/GenericUsb/GenericUsbInput.h"

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
		ReplNau::Lua::init();
		ReplNau::Hook::init();
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

	bool DeviceConnected = true;
	bool FirstUpdateTick = true;
	bool HasWindowFocus, HadWindowFocus;

	void MainModule::InitializeTick()
	{
		MainModule::DivaWindowHandle = WindowFromDC(wglGetCurrentDC());

		HRESULT diInitResult = Input::InitializeDirectInput(MainModule::Module);
		if (FAILED(diInitResult))
			printf("[ReplNau] InitializeTick(): Failed to initialize DirectInput. Error: 0x%08X\n", diInitResult);
	}

	void MainModule::UpdateTick()
	{
		if (FirstUpdateTick)
		{
			FirstUpdateTick = false;
			MainModule::InitializeTick();
		}

		if (DeviceConnected)
		{
			DeviceConnected = false;

			if (!Input::DualShock4::InstanceInitialized())
			{
				if (Input::DualShock4::TryInitializeInstance())
					printf("[ReplNau] UpdateTick(): DualShock4 connected and initialized\n");
			}

			if (!Input::GenericUsbInput::InstanceInitialized())
			{
				if (Input::GenericUsbInput::TryInitializeInstance())
					printf("[ReplNau] UpdateTick(): GenericUsbInput connected and initialized\n");
			}
		}

		HasWindowFocus = MainModule::DivaWindowHandle == nullptr || GetForegroundWindow() == MainModule::DivaWindowHandle;

		if (HasWindowFocus)
		{
			Input::Keyboard::GetInstance()->PollInput();
			Input::Mouse::GetInstance()->PollInput();
			Input::Xinput::GetInstance()->PollInput();

			if (Input::DualShock4::GetInstance() != nullptr)
			{
				if (!Input::DualShock4::GetInstance()->PollInput())
				{
					Input::DualShock4::DeleteInstance();
					printf("[ReplNau] UpdateTick(): DualShock4 connection lost\n");
				}
			}

			if (Input::GenericUsbInput::GetInstance() != nullptr)
			{
				if (!Input::GenericUsbInput::GetInstance()->PollInput())
				{
					Input::GenericUsbInput::DeleteInstance();
					printf("[ReplNau] UpdateTick(): GenericUsbInput connection lost\n");
				}
			}
		}
	}

	void deinitialize()
	{
		delete Input::Keyboard::GetInstance();
		delete Input::Mouse::GetInstance();
		delete Input::DualShock4::GetInstance();
		delete Input::GenericUsbInput::GetInstance();

		Input::DisposeDirectInput();
	}
}