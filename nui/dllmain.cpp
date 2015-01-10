// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <Windows.h>
#include "./data/NModule.h"
#include "./base/NReflect.h"

BOOL _stdcall DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    UNREFERENCED_PARAMETER(lpReserved);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        {
            nui::Data::NModule::GetInst().Init(hModule);
            break;
        }
    case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

