#include <filesystem>
#include "Lua.h"
#include <sol/sol.hpp>

#include "../Constants.h"
#include "../MainModule.h"

#include "../Struct/DivaModel.h"

namespace fs = std::filesystem;

namespace ReplNau {
	sol::state* Lua::state = nullptr;

	void bind_aft_globals(sol::state& lua) {
		sol::table aft = lua.create_table();

		Ext::DivaModel* model_1p = (Ext::DivaModel*)MODEL_1P_ADDRESS;
		Ext::DivaModel* model_2p = (Ext::DivaModel*)MODEL_2P_ADDRESS;
		
		aft["MODEL_1P"] = model_1p;
		aft["MODEL_2P"] = model_2p;

		lua["aft"] = aft;
	}

	void Lua::init() {
		assert(!Lua::state);
		Lua::state = new sol::state();
		state->open_libraries(sol::lib::base);

		bind_aft_globals(*state);

		std::string module_path = MainModule::GetModuleDirectory();
		fs::path init_file(module_path);
		init_file = init_file / "repl" / "init.lua";

		auto result = state->safe_script_file(init_file.string());
		if (!result.valid()) {
			sol::error err = result;
			printf("[ReplNau] Failed to run %ws: %s\n", init_file.c_str(), err.what());
		}

		printf("[ReplNau] Initialized Lua\n");
	}
	
	void Lua::deinit() {
		assert(Lua::state);
		delete Lua::state;
		Lua::state = nullptr;
	}
}