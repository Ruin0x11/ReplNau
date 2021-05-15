#include "ExtraBindings.h"
#include "Config.h"
#include "../../FileSystem/ConfigFile.h"
#include "../../Lua/Lua.h"

namespace ReplNau::Input::KeyConfig {
	ExtraBindings::BindingMap ExtraBindings::Bindings;

	void ExtraBindings::Bind(const char* key, const char* key_name) {
		auto it = ExtraBindings::Bindings.find(key_name);
		Binding* binding;
		if (it != ExtraBindings::Bindings.end()) {
			binding = it->second;
		}
		else {
			auto key = KeyString(key_name);
			ExtraBindings::Bindings.insert(std::make_pair(key, new Binding()));
			binding = ExtraBindings::Bindings[key_name];
		}

		std::unordered_map<std::string, std::string> configMap;
		Config::BindConfigKeys(configMap, key_name, *binding, { key });
	}

	void ExtraBindings::Unbind(const char* key_name) {
		auto it = ExtraBindings::Bindings.find(key_name);
		if (it == ExtraBindings::Bindings.end()) {
			return;
		}

		auto binding = it->second;
		ExtraBindings::Bindings.erase(it);
		delete binding;
	}

	void ExtraBindings::Update() {
		for (auto& it : ExtraBindings::Bindings) {
			auto& key_name = it.first;
			Binding* binding = it.second;
			if (binding->AnyTapped()) {
				Lua::trigger_event("keybind", key_name.value, "down");
			}
			if (binding->AnyReleased()) {
				Lua::trigger_event("keybind", key_name.value, "up");
			}
		}
	}
}