// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Global.h"
#include "Phys.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		//HotReloading::GetInstance();
		//HotReloading::RegClass("RigidBody", CreateRigidBody);
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		//HotReloading::DeInit();
		//HotReloading::NukeClass();
		DebugConsole::DestroyConsole();
        break;
    }
    return TRUE;
}

