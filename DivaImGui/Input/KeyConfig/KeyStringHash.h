#pragma once
#include "KeyString.h"

namespace ReplNau::Input::KeyConfig
{
	struct KeyStringHash
	{
		size_t operator()(const KeyString& key) const;
	};
}
