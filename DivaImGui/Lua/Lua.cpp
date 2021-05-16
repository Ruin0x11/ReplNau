#include <filesystem>
#include "Lua.h"

#include "../Constants.h"
#include "../PV_Param/PVParam701.h"
#include "../MainModule.h"
#include "../Input/KeyConfig/ExtraBindings.h"
#include "../Utilities/MemRightsLock.h"

#include "../Struct/Camera.h"
#include "../Struct/DivaModel.h"

namespace fs = std::filesystem;

namespace ReplNau {
	std::unique_ptr<sol::state> Lua::state = nullptr;

	void bind_lib_C(sol::state& lua) {
		sol::table C = lua.create_table();

		C["peek"] = lua_CFunction([](lua_State* L) {
			// buf = peek(addr, len)
			size_t addr = (size_t)lua_touserdata(L, 1);
			if (addr == 0) addr = luaL_checkinteger(L, 1);
			size_t scanmax = luaL_checkinteger(L, 2);
			char* buf = (char*)alloca(scanmax);
			SIZE_T gotread;

			if (!ReadProcessMemory(GetCurrentProcess(), (void*)addr, buf, scanmax, &gotread))
				return 0;
			char* p = (char*)addr;
			size_t i = scanmax;

			if (lua_gettop(L) > 2) {
				size_t untilsz;
				size_t step = 1;
				const char* until = luaL_checklstring(L, 3, &untilsz);
				if (lua_gettop(L) > 3)
					step = lua_tointeger(L, 4);
				if (step < 1)
					step = 1;

				for (i = 0; i < scanmax; i += step) {
					if (!memcmp(p + i, until, untilsz))
						break;
				}
			}

			lua_pushlstring(L, p, i);
			return 1;
			});

		C["poke"] = lua_CFunction([](lua_State* L) {
			// poke(addr, buf)
			size_t addr = luaL_checkinteger(L, 1);
			if (!addr)
				addr = (size_t)lua_topointer(L, 1);
			size_t nbytes;
			const char* buf = luaL_checklstring(L, 2, &nbytes);
			void* ptr = (void*)(addr);
			{
				MemRights unprotect(ptr, nbytes);
				size_t wrote = 0;
				WriteProcessMemory(GetCurrentProcess(), ptr, buf, nbytes, &wrote);
				lua_pushinteger(L, wrote);
			}
			FlushInstructionCache(GetCurrentProcess(), 0, 0);
			return 1;
			});

		lua["C"] = C;
	}

	void bind_lib_input(sol::state& lua) {
		sol::table input = lua.create_table();

		input["_bind"] = Input::KeyConfig::ExtraBindings::Bind;
		input["_unbind"] = Input::KeyConfig::ExtraBindings::Unbind;

		lua["input"] = input;
	}

	void bind_pdaft_types(sol::state& lua) {
		Utilities::Vec2::Bind(lua);
		Utilities::Vec3::Bind(lua);
		
		PVParam::Bind(lua);

		Ext::Camera::Bind(lua);
		Ext::DivaModel::Bind(lua);
	}

	void bind_pdaft_globals(sol::state& lua) {
		sol::table pdaft = lua.create_table();

		pdaft["get_module_directory"] = MainModule::GetModuleDirectory;

		{
			sol::table global = lua.create_table();

			global["camera"] = (Ext::Camera*)CAMERA_ADDRESS;

			global["model_1p"] = (Ext::DivaModel*)MODEL_1P_ADDRESS;
			global["model_2p"] = (Ext::DivaModel*)MODEL_2P_ADDRESS;

			global["fog"] = PVParam::FogParam;
			global["light"] = PVParam::LTParam;
			global["post_process"] = PVParam::PPParam;
			global["fog_timed"] = PVParam::FGdata;
			global["light_timed"] = PVParam::LPdata;

			pdaft["G"] = global;
		}

		lua["pdaft"] = pdaft;
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

		bind_lib_C(*state);
		bind_lib_input(*state);

		bind_pdaft_types(*state);
		bind_pdaft_globals(*state);

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