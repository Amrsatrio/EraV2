/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#include <windows.h>
#include "Internals.h"
#include "OffsetInitialize.h"
#include "UnrealEngineStructs.h"
#include "CURLhook.h"

DWORD _stdcall FortniteMain(LPVOID)
{
    void* Result;
    EraInit::Console();
    string Receive = EngineVersion::GetVersion(&Result);
    EraInit::SetOffsets(Receive);
    CURLhook::StartHook();
    return NULL;
}

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH: {
        CreateThread(0, 0, FortniteMain, 0, 0, 0);
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

