#pragma once
#include <Windows.h>

/*
 * A RAIR class that manages memory access rights
 */

namespace ReplNau {
	class MemRights {
	private:
		DWORD oldRights;
		LPVOID addr;
		SIZE_T size;
	public:
		bool good;

		inline MemRights(LPVOID addr, SIZE_T size) : addr(addr), size(size) {
			good = VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &(this->oldRights)) == TRUE;
		}
		inline ~MemRights() {
			DWORD tmp;
			if (good) VirtualProtect(addr, size, oldRights, &tmp);
		}
	};
}
