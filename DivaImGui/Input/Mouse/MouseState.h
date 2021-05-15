#pragma once
#include <windows.h>

namespace ReplNau::Input
{
	struct MouseState
	{
		POINT Position;
		POINT RelativePosition;
		long MouseWheel;
		bool ScrolledUp;
		bool ScrolledDown;
	};
}

