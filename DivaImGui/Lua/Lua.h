#pragma once

#include <sol/sol.hpp>
#include <memory>

namespace ReplNau {
	class Lua {
	public:
		static void init();
		static void deinit();

		template<typename... Args>
		static void trigger_event(const char* name, Args... args) {
			sol::state& lua = *Lua::state;
			sol::protected_function trigger = lua["event"]["trigger"];
			auto result = trigger(name, args...);
			if (!result.valid()) {
				sol::error err = result;
				printf("[ReplNau] Failed to run Lua event %s: %s\n", name, err.what());
			}
		}

	public:
		static std::unique_ptr<sol::state> state;
	};
}