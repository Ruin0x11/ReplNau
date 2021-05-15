#include <filesystem>
#include "Lua.h"
#include <sol/sol.hpp>

#include "../Constants.h"
#include "../PV_Param/PVParam701.h"
#include "../MainModule.h"

#include "../Struct/DivaModel.h"
#include "../Input/KeyConfig/ExtraBindings.h"

namespace fs = std::filesystem;

namespace ReplNau {
	std::unique_ptr<sol::state> Lua::state = nullptr;

	void bind_aft_globals(sol::state& lua) {
		sol::table aft = lua.create_table();

		aft["get_module_directory"] = MainModule::GetModuleDirectory;

		{
			sol::table global = lua.create_table();
			Ext::DivaModel* model_1p = (Ext::DivaModel*)MODEL_1P_ADDRESS;
			Ext::DivaModel* model_2p = (Ext::DivaModel*)MODEL_2P_ADDRESS;

			global["model_1p"] = model_1p;
			global["model_2p"] = model_2p;
			global["light"] = PVParam::LTParam;
			global["post_process"] = PVParam::PPParam;
			global["fog"] = PVParam::FogParam;
			
			aft["G"] = global;
		}

		lua["aft"] = aft;
	}

	void bind_lib_input(sol::state& lua) {
		sol::table input = lua.create_table();

		input["bind"] = Input::KeyConfig::ExtraBindings::Bind;
		input["unbind"] = Input::KeyConfig::ExtraBindings::Unbind;

		lua["input"] = input;
	}

	void Lua::init() {
		assert(!Lua::state);
		Lua::state = std::make_unique<sol::state>(sol::state());
		state->open_libraries(
			sol::lib::base,
			sol::lib::package,
			sol::lib::coroutine,
			sol::lib::string,
			sol::lib::os,
			sol::lib::math,
			sol::lib::table,
			sol::lib::debug,
			sol::lib::bit32,
			sol::lib::io,
			sol::lib::ffi,
			sol::lib::jit);

		bind_aft_globals(*state);

		bind_lib_input(*state);

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
		Lua::state.release();
	}
}