/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef PROCESSEVENT_H
#define PROCESSEVENT_H

#include <Windows.h>
#include "UnrealEngineStructs.h"
#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/libMinHook.x64.lib")
#include "Internals.h"
#include "OffsetTable.h"
#include "Functions.h"

GameManager ClassInstance;
bool bMatchStarted = false;
bool bLoadingScreenDropped = false;

PVOID ProcessEventHook(UObject* A, UObject* B, PVOID C)
{
	if (B->GetObjectNameString().find(XORSTRING("ReadyToStartMatch")) != string::npos && Globals::bPressedPlay && !bMatchStarted)
	{
		ClassInstance.Init();
		bMatchStarted = true;
	}
	if (B->GetObjectNameString().find(XORSTRING("LoadingScreenDropped")) != string::npos && Globals::bPressedPlay && bMatchStarted && !bLoadingScreenDropped)
	{
		ClassInstance.OnLoadingScreenDropped();
		bLoadingScreenDropped = true;
	}
	if (B->GetObjectNameString().find(XORSTRING("Tick")) != string::npos && bLoadingScreenDropped)
	{
		ClassInstance.Tick();
	}
	if ((B->GetObjectNameString().find(XORSTRING("AttemptAircraftJump")) != string::npos && bLoadingScreenDropped) || (B->GetObjectNameString().find(XORSTRING("ExitedDropZone")) != string::npos && bLoadingScreenDropped))
	{
		if (!Globals::bJumpedFromBus)
		{
			ClassInstance.JumpFromAircraft();
			Globals::bJumpedFromBus = true;
		}
	}
	if (B->GetObjectNameString().find(XORSTRING("HandleQuickbarSlotFocusSlotChanged")) != string::npos && bLoadingScreenDropped)
	{
		ClassInstance.EquipQuickbarItem(reinterpret_cast<SlotFocusParams*>(C)->QuickbarIndex, reinterpret_cast<SlotFocusParams*>(C)->Slot);
	}

	return Globals::ProcessEvent(A, B, C);
}

void InitializePEH()
{
	MH_CreateHook(static_cast<LPVOID>(OffsetTable::ProcessEvent), ProcessEventHook, reinterpret_cast<LPVOID*>(&Globals::ProcessEvent));
	MH_EnableHook(static_cast<LPVOID>(OffsetTable::ProcessEvent));

}



#endif // !PROCESSEVENT_H

