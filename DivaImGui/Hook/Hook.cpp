#include <MinHook.h>
#include <detours/detours.h>
#include <stdio.h>

#include "Hook.h"

namespace ReplNau {
	typedef void(__cdecl* ftdivaEngineUpdate)();

	ftdivaEngineUpdate divaEngineUpdate;

	void hookedDivaEngineUpdate() {
		printf("[ReplNau] Diva Engine Update\n");
		divaEngineUpdate();
	}

    void Hook::init() {
		void* ptr = (ftdivaEngineUpdate)0x14018CC40;
		MH_Initialize();
		MH_STATUS stat = MH_CreateHook(ptr, hookedDivaEngineUpdate, reinterpret_cast<void**>(&divaEngineUpdate));
		if (stat != MH_OK)
		{
			printf("[ReplNau] Hook: %s\n", MH_StatusToString(stat));
			return;
		}

		MH_EnableHook(ptr);
		printf("[ReplNau] Hooked\n");
	}
}