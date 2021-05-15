#pragma once

namespace ReplNau::Input
{
	class IInputDevice
	{
	public:
		virtual bool PollInput() = 0;
	};
}
