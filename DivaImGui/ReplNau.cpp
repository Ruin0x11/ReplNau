// ReplNau.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MainModule.h"


#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT LPCWSTR GetPluginName() {
	return L"ReplNau";
}

EXTERN_DLL_EXPORT LPCWSTR GetPluginDescription() {
	return L"ReplNau\nReplNau";
}

EXTERN_DLL_EXPORT int getVersion() {
	return 1;
}