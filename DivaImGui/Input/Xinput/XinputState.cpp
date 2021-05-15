#include "XinputState.h"

namespace ReplNau::Input
{
	bool XinputState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}