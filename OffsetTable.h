/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef OFFSETTABLE_H
#define OFFSETTABLE_H

#include <Windows.h>
#include "LeoSpecial.h"
#include "UnrealEngineStructs.h"

uintptr_t base_address = (uintptr_t)GetModuleHandle(NULL);

struct Universal
{
	PBYTE addr_GetEngineVersion;
	LeoHook CURLhook;
	PBYTE CurlPattern;
	PBYTE CurlPatternSet;
	int CurrentVersion;
	void* Result;
};

namespace OffsetTable {
	FUObjectArray* GObjects = nullptr;
	int GetObjectName = 0;
	PBYTE ProcessEvent = 0;
	int UEngine = 0;
	int ViewPort = 0;
	int World = 0;
	int GameInstance = 0;
	int LocalPlayers = 0;
	int PlayerController = 0;
	int GameMode = 0;
	int GameState = 0;
	int MinimapBrush = 0;
	int MinimapSecondBrush = 0;
	int MinimapThirdBrush = 0;
	int MinimapFourthBrush = 0;
	int MinimapFifthBrush = 0;
	int MinimapSixthBrush = 0;
	int MinimapSeventhBrush = 0;
	int StrongMyHero = 0;
	int CharacterParts = 0;
	int AdditionalData = 0;
	int SprintingBool = 0;
	int MovementStyle = 0;
	int Inventory = 0;
	int QuickBar = 0;
	int ItemEntries = 0;
	int ItemInstances = 0;
	int ItemEntry = 0;
	PBYTE SpawnActor = 0;
}

namespace Globals
{
	UObject* PlayerController;
	UObject* World;
	UObject* PlayerPawn;
	UObject* GameMode;
	UObject* GameState;
	UObject* CheatManager;
	UObject* PlayerState;
	UObject* GameplayStatics;
	UObject* Quickbar;
	UObject* Inventory;
	UObject* InventoryContext; 

	PVOID(*ProcessEvent)(UObject*, UObject*, PVOID) = nullptr;
	bool bPressedPlay = false;
	bool bJumped = false;
	bool bJumpedFromBus = false;
}

namespace Functions
{
	UObject* CanJump;
	UObject* Jump;
	UObject* DestroyActor;
	UObject* Summon;
	UObject* SetPart;
	UObject* OnRep_CharacterParts;
	UObject* GetActorsOfClass;
	UObject* ReplicateLoadout;
	UObject* SetRotation;
	UObject* GetRotation;
	UObject* EquipWeapon;
	UObject* GetQuickbarItem; 
	UObject* GetGuid;
	UObject* GetDefinition;
	UObject* GetLocation;
}

namespace Skin
{
	UObject* Head;
	UObject* Body;
	UObject* Hat;
	UObject* Backpack;
}

auto UniversalStructInstance = new Universal;



struct EngineToViewport
{
	
};

#endif // !OFFSETTABLE_H