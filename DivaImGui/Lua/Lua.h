#pragma once

#include <sol/forward.hpp>

namespace ReplNau {
	class Lua {
	public:
		static void init();
		static void deinit();

		static sol::state* state;
	};
}