#include "../stdafx.h"
#include <detours/detours.h>
#include <stdio.h>

#include "../framework.h"
#include "../MainModule.h"
#include "Hook.h"
#include "../Lua/Lua.h"

#include "../Input/KeyConfig/ExtraBindings.h"

namespace ReplNau {
	//HOOK(void*, IMFTransformInitializer, 0x140420B90, void* a1, void* a2, IMFTransform** transform)
	//{
	//	return result;
	//}

	HOOK(void, pvMenuInit, 0x1405c4400, __int64 a1) {
		Lua::trigger_event("pv_menu_init");
		pvMenuInit(a1);
	}

	HOOK(void, engineTickFrame, 0x140192230) {
		MainModule::UpdateTick();
		Input::KeyConfig::ExtraBindings::Update();
		Lua::trigger_event("tick");
		engineTickFrame();
	}

    void Hook::init() {
		Lua::trigger_event("init");

		INSTALL_HOOK(pvMenuInit);
		INSTALL_HOOK(engineTickFrame);

		printf("[ReplNau] Hooked\n");
	}
}