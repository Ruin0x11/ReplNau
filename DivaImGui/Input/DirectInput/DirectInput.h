#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace ReplNau::Input
{
	extern IDirectInput8 *IDirectInputInstance;

	HRESULT InitializeDirectInput(HMODULE module);
	bool DirectInputInitialized();
	void DisposeDirectInput();
}