#pragma once

#define PRINT(value, ...) if (true) printf(value, __VA_ARGS__);

#define VTABLE_HOOK(returnType, className, functionName, ...) \
	typedef returnType functionName(className* This, __VA_ARGS__); \
	functionName* original##functionName; \
	returnType implOf##functionName(className* This, __VA_ARGS__)

#define INSTALL_VTABLE_HOOK(object, functionName, functionIndex) \
	{ \
		void** addr = &(*(void***)object)[functionIndex]; \
		if (*addr != implOf##functionName) \
		{ \
			PRINT("[ReplNau] Installing %s hook...\n", #functionName); \
			original##functionName = (functionName*)*addr; \
			DWORD oldProtect; \
			VirtualProtect(addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); \
			*addr = implOf##functionName; \
			VirtualProtect(addr, sizeof(void*), oldProtect, NULL); \
		} \
	}

#define HOOK(returnType, function, location, ...) \
    typedef returnType ft##function(__VA_ARGS__); \
    ft##function* function = (ft##function*)location; \
    returnType implOf##function(__VA_ARGS__)

#define INSTALL_HOOK(functionName) \
	{ \
		PRINT("[ReplNau] Installing %s hook...\n", #functionName); \
		DetourTransactionBegin(); \
		DetourUpdateThread(GetCurrentThread()); \
		DetourAttach((void**)&functionName, implOf##functionName); \
		DetourTransactionCommit(); \
	}