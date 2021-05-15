#pragma once
#include <windows.h>

namespace ReplNau::Input
{
	struct XinputState
	{
		bool KeyStates[0x8F];

		bool IsDown(BYTE keycode);
	};
}
