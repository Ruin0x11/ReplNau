#include "KeyboardState.h"

namespace ReplNau::Input
{
	bool KeyboardState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}
