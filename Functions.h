/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Windows.h>
#include "UnrealEngineStructs.h"
#include "OffsetInitialize.h"
#include "OffsetTable.h"
#include "GameFunctions.h"

class GameManager
{
public:

	void InitializeGlobals()
	{
		UObject* Engine;
		Engine = *reinterpret_cast<UObject**>(base_address + OffsetTable::UEngine);
		auto ViewportClient = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Engine) + OffsetTable::ViewPort);
		auto GameInstance = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(ViewportClient) + OffsetTable::GameInstance);
		auto LocalPlayers = *reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(GameInstance) + OffsetTable::LocalPlayers);
		Globals::PlayerController = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(LocalPlayers[0]) + OffsetTable::PlayerController);
		Globals::World = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(ViewportClient) + OffsetTable::World);
		Globals::GameMode = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::World) + OffsetTable::GameMode);
		Globals::GameState = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::World) + OffsetTable::GameState);
	}

	void OpenAthenaTerrain()
	{
		SwitchLevel();
	}

	void Init()
	{
		InitializeObjects();
		InitializeGlobals();
		Globals::PlayerPawn = SpawnActorFromClass(XORSTRING("BlueprintGeneratedClass PlayerPawn_Athena.PlayerPawn_Athena_C"), FVector{ -128535, -110908, 3916 });
		Possess();
		EquipSkin();
		MiniMap();
		DestroyLods();
		SetName();
		CustomizationLoadout();
		Inventory();
		DropLoadingScreen();
	}

	void OnLoadingScreenDropped()
	{
		HideNetDebugUI();
		EquipWeapon(GetDefinition(GetQuickbarItem(EFortQuickBars::Primary, 0)), GetGuid(GetQuickbarItem(EFortQuickBars::Primary, 0)));
	}

	void EquipQuickbarItem(EFortQuickBars QuickbarIndex, int Slot)
	{
		EquipWeapon(GetDefinition(GetQuickbarItem(QuickbarIndex, Slot)), GetGuid(GetQuickbarItem(QuickbarIndex, Slot)));
	}

	void JumpFromAircraft()
	{
		DestroyPawn(); 
		Summon(XORSTRING(TEXT("PlayerPawn_Athena_C")));
		Globals::PlayerPawn = FindActorsFromClass(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.FortPlayerPawnAthena")))[0];
		Possess();
		ReEquipSkin();
		AdjustRotation();
		CustomizationLoadout();
	}

	void Tick()
	{
		//ON 5.10, to patch sprinting, hook a function and make it return 0.
		//Function address on 5.10 : baseaddress + 0x1107A30

		auto MovementStyle = reinterpret_cast<EFortMovementStyle*>(reinterpret_cast<uintptr_t>(Globals::PlayerPawn) + OffsetTable::MovementStyle);

		if (*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(Globals::PlayerController) + OffsetTable::SprintingBool))
		{
			*MovementStyle = EFortMovementStyle::Sprinting;
		}
		else *MovementStyle = EFortMovementStyle::Running;

		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			if (!Globals::bJumped)
			{
				Globals::bJumped = true;
				if (CanJump())
				{
					Jump();
				}
			}
		}
		else
		{
			Globals::bJumped = false;
		}
	}
};


#endif // !FUNCTIONS_H
