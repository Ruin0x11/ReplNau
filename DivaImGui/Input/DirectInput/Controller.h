#pragma once
#include "DirectInputDevice.h"

namespace ReplNau::Input
{
	class Controller : public DirectInputDevice
	{
	protected:
		Controller();
		~Controller();
	};
}