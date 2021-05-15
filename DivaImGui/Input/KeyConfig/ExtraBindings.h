#pragma once

#include "../Bindings/IInputBinding.h"
#include "../Bindings/Binding.h"
#include "KeyString.h"
#include "KeyStringHash.h"
#include <unordered_map>

namespace ReplNau::Input::KeyConfig
{
	class ExtraBindings
	{
	public:
		static void Bind(const char* key_name, const char* key);
		static void Unbind(const char* key_name);
		static void Update();

	public:
		typedef std::unordered_map<KeyString, Binding*, KeyStringHash> BindingMap;
		static BindingMap Bindings;
	};
}
