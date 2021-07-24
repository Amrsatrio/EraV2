/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include "UnrealEngineStructs.h"
#include "Internals.h"
#include "OffsetTable.h"
#include <array>

void SwitchLevel()
{
	struct SwitchLevel_Params
	{
		FString URL;
	};

	SwitchLevel_Params params;
	params.URL = XORSTRING(L"Athena_Terrain");

	Globals::ProcessEvent(Globals::PlayerController, UObject::GetObjectFromName(XORSTRING("Function Engine.PlayerController.SwitchLevel")), &params);
}

UObject* SpawnActorFromClass(string ClassName, FVector pPosition)
{
	auto Class = UObject::GetObjectFromName(ClassName);
	auto Position = pPosition;
	auto Rotation = new FRotator{};

	struct SpawnActorParams
	{
		unsigned char Bytes[0x40];
	};
	UObject* (*SpawnActor)(UObject* World, UObject* Class, FVector* Location, FRotator* Rotation, SpawnActorParams& SpawnParameters);
	SpawnActor = reinterpret_cast<decltype(SpawnActor)>(OffsetTable::SpawnActor + 5 + *reinterpret_cast<uint32_t*>(OffsetTable::SpawnActor + 1));

	SpawnActorParams params{};

	auto Actor = SpawnActor(Globals::World, Class, &Position, Rotation, params);
	return Actor;
}

void Possess()
{
	struct Possess_Params
	{
		UObject* Pawn;
	};

	Possess_Params params;
	params.Pawn = Globals::PlayerPawn;

	Globals::ProcessEvent(Globals::PlayerController, UObject::GetObjectFromName(XORSTRING("Function Engine.Controller.Possess")), &params);
}

void DropLoadingScreen()
{
	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerController, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerController.ServerReadyToStartMatch")), &params);
	Globals::ProcessEvent(Globals::GameMode, UObject::GetObjectFromName(XORSTRING("Function Engine.GameMode.StartMatch")), &params);
	Globals::ProcessEvent(UObject::GetObjectFromName(XORSTRING("FortPlayerStateAthena Athena_Terrain.Athena_Terrain.PersistentLevel.FortPlayerStateAthena_0_1")), UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerStateAthena.OnRep_SquadId")), &params);
}

void InitializeObjects()
{
	Globals::GameplayStatics = UObject::GetObjectFromName(XORSTRING("GameplayStatics Engine.Default__GameplayStatics"));
	Globals::CheatManager = UObject::GetObjectFromName(XORSTRING("FortCheatManager Athena_Terrain.Athena_Terrain.PersistentLevel.Athena_PlayerController_C_0_1.FortCheatManager_0_1"));
	Globals::PlayerState = UObject::GetObjectFromName(XORSTRING("FortPlayerStateAthena Athena_Terrain.Athena_Terrain.PersistentLevel.FortPlayerStateAthena_0_1"));
	Globals::InventoryContext = UObject::GetObjectFromName(XORSTRING("FortInventoryContext Transient.FortEngine_0_1.FortLocalPlayer_0_1.FortInventoryContext_0_1"));
	Functions::CanJump = UObject::GetObjectFromName(XORSTRING("Function Engine.Character.CanJump"));
	Functions::Jump = UObject::GetObjectFromName(XORSTRING("Function Engine.Character.Jump"));
	Functions::GetDefinition = UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortItem.GetItemDefinitionBP"));
	Functions::DestroyActor = UObject::GetObjectFromName(XORSTRING("Function Engine.Actor.K2_DestroyActor"));
	Functions::Summon = UObject::GetObjectFromName(XORSTRING("Function Engine.CheatManager.Summon"));
	Functions::SetPart = UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerPawn.ServerChoosePart"));
	Functions::OnRep_CharacterParts = UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerState.OnRep_CharacterParts"));
	Functions::GetActorsOfClass = UObject::GetObjectFromName(XORSTRING("Function Engine.GameplayStatics.GetAllActorsOfClass"));
	Functions::ReplicateLoadout = UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerPawnAthena.OnRep_CustomizationLoadout"));
	Functions::SetRotation = UObject::GetObjectFromName(XORSTRING("Function Engine.Actor.K2_SetActorRotation"));
	Functions::GetRotation = UObject::GetObjectFromName(XORSTRING("Function Engine.Actor.K2_GetActorRotation"));
	Functions::EquipWeapon = UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPawn.EquipWeaponDefinition"));
	Functions::GetQuickbarItem = UObject::GetObjectFromName(XORSTRING("Function FortniteUI.FortInventoryContext.GetQuickBarSlottedItem"));
	Functions::GetGuid = UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortItem.GetItemGuid"));
	Functions::GetLocation = UObject::GetObjectFromName(XORSTRING("Function Engine.Actor.K2_GetActorLocation"));
	
}

void DestroyLods()
{
	if (UniversalStructInstance->CurrentVersion > 1)
	{
		struct DestroyLods_Params
		{
			UObject* Class;
		};
		DestroyLods_Params params;
		params.Class = UObject::GetObjectFromName(XORSTRING("Class FortniteGame.FortHLODSMActor"));
		
		Globals::ProcessEvent(Globals::CheatManager, UObject::GetObjectFromName(XORSTRING("Function Engine.CheatManager.DestroyAll")), &params);
	}
}

void MiniMap()
{
	reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapBrush)->ObjectResource = UObject::GetObjectFromName(XORSTRING("Texture2D MiniMapAthena.MiniMapAthena"));
	if (UniversalStructInstance->CurrentVersion > 1)
	{
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSecondBrush)->Margin = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSecondBrush)->ImageSize = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSecondBrush)->TintColor = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSecondBrush)->ObjectResource = {};
	}
	if (UniversalStructInstance->CurrentVersion > 2)
	{
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapThirdBrush)->Margin = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapThirdBrush)->ImageSize = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapThirdBrush)->TintColor = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapThirdBrush)->ObjectResource = {};

		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFourthBrush)->Margin = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFourthBrush)->ImageSize = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFourthBrush)->TintColor = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFourthBrush)->ObjectResource = {};

		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFifthBrush)->Margin = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFifthBrush)->ImageSize = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFifthBrush)->TintColor = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapFifthBrush)->ObjectResource = {};

		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSixthBrush)->Margin = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSixthBrush)->ImageSize = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSixthBrush)->TintColor = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSixthBrush)->ObjectResource = {};

		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSeventhBrush)->Margin = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSeventhBrush)->ImageSize = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSeventhBrush)->TintColor = {};
		reinterpret_cast<FSlateBrush*>(reinterpret_cast<uintptr_t>(Globals::GameState) + OffsetTable::MinimapSeventhBrush)->ObjectResource = {};
	}
}

void SetName()
{
	struct ChangeName_Params
	{
		UObject* PlayerController;
		FString Name;
		bool UpdateName;
	};

	ChangeName_Params params;
	params.PlayerController = Globals::PlayerController;
	params.Name = TEXT("PROJECT ERA");
	params.UpdateName = true;

	struct ServerChangeName_Params
	{
		FString Name;
	};

	ServerChangeName_Params params2;
	params2.Name = TEXT("PROJECT ERA");

	Globals::ProcessEvent(Globals::GameMode, UObject::GetObjectFromName(XORSTRING("Function Engine.GameModeBase.ChangeName")), &params);
	Globals::ProcessEvent(Globals::PlayerController, UObject::GetObjectFromName(XORSTRING("Function Engine.PlayerController.ServerChangeName")), &params2);
}

void ServerChoosePart(TEnumAsByte<EFortCustomPartType> Type, UObject* Part)
{

	struct ServerChoosePart_Params
	{
		TEnumAsByte<EFortCustomPartType> Part;
		UObject* ChosenCharacterPart;
	};

	ServerChoosePart_Params params;
	params.Part = Type;
	params.ChosenCharacterPart = Part;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::SetPart, &params);
}

void EquipSkin()
{
	std::vector<UObject*> CharacterPartsArray;

	auto FortHero = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(Globals::PlayerController) + OffsetTable::StrongMyHero);
	auto HeroCharacterParts = *reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(FortHero) + OffsetTable::CharacterParts);

	for (auto i = 0; i < HeroCharacterParts.Num(); i++)
		CharacterPartsArray.push_back(HeroCharacterParts[i]);



	for (auto i = 0; i < CharacterPartsArray.size(); i++)
	{
		auto AdditionalData = *reinterpret_cast<UObject**>(reinterpret_cast<uintptr_t>(CharacterPartsArray[i]) + OffsetTable::AdditionalData);
		if (AdditionalData->IsA(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.CustomCharacterHeadData"))))
		{
			ServerChoosePart(EFortCustomPartType::Head, CharacterPartsArray[i]);
			Skin::Head = CharacterPartsArray[i];
		}
		else if (AdditionalData->IsA(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.CustomCharacterBodyPartData"))))
		{
			ServerChoosePart(EFortCustomPartType::Body, CharacterPartsArray[i]);
			Skin::Body = CharacterPartsArray[i];
		}
		else if (AdditionalData->IsA(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.CustomCharacterHatData"))))
		{
			ServerChoosePart(EFortCustomPartType::Hat, CharacterPartsArray[i]);
			Skin::Hat = CharacterPartsArray[i];
		}
		else if (AdditionalData->IsA(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.CustomCharacterBackpackData"))))
		{
			ServerChoosePart(EFortCustomPartType::Backpack, CharacterPartsArray[i]);
			Skin::Backpack = CharacterPartsArray[i];
		}
	}

	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerState, Functions::OnRep_CharacterParts, &params);
}

void ReEquipSkin()
{
	ServerChoosePart(EFortCustomPartType::Head, Skin::Head);
	ServerChoosePart(EFortCustomPartType::Body, Skin::Body);
	ServerChoosePart(EFortCustomPartType::Hat, Skin::Hat);
	ServerChoosePart(EFortCustomPartType::Backpack, Skin::Backpack);

	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerState, Functions::OnRep_CharacterParts, &params);
}

void HideNetDebugUI()
{
	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(UObject::FindPartOfObject(XORSTRING("NetDebugUI_C Transient.FortEngine_0_1.FortGameInstance_0_1.AthenaHUD_C_0_1.WidgetTree_0_1.NetDebugContainer.WidgetTree_")), UObject::GetObjectFromName(XORSTRING("Function UMG.UserWidget.RemoveFromViewport")), &params);
}

bool CanJump()
{
	struct CanJump_Params
	{
		bool ReturnValue;
	};

	CanJump_Params params;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::CanJump, &params);

	return params.ReturnValue;
}

void Jump()
{
	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::Jump, &params);
}

void DestroyPawn()
{
	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::DestroyActor, &params);
}

void Summon(FString Parameter)
{
	struct Summon_Params
	{
		FString Object;
	};

	Summon_Params params;
	params.Object = Parameter;

	Globals::ProcessEvent(Globals::CheatManager, Functions::Summon, &params);
}

TArray<UObject*> FindActorsFromClass(UObject* Class)
{
	TArray<UObject*> ActorArray;

	struct FindActor_Params
	{
		UObject* WorldContextObject;
		UObject* ActorClass;
		TArray<UObject*> ReturnValue;
	};

	FindActor_Params params;
	params.WorldContextObject = Globals::World;
	params.ActorClass = Class;

	Globals::ProcessEvent(Globals::GameplayStatics, Functions::GetActorsOfClass, &params);

	ActorArray = params.ReturnValue;

	return ActorArray;
}

FRotator GetRotation()
{
	struct GetRotation_Params
	{
		FRotator ReturnValue;
	};

	GetRotation_Params params;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::GetRotation, &params);

	return params.ReturnValue;
}

FVector GetLocation()
{
	struct GetLocation_Params
	{
		FVector ReturnValue;
	};

	GetLocation_Params params;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::GetLocation, & params);

	return params.ReturnValue;
}

FVector GetAircraftLocation()
{
	struct GetLocation_Params
	{
		FVector ReturnValue;
	};

	GetLocation_Params params;

	Globals::ProcessEvent(FindActorsFromClass(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.FortAthenaAircraft")))[0], Functions::GetLocation, &params);

	return params.ReturnValue;
}

FRotator GetAircraftRotation()
{
	struct GetRotation_Params
	{
		FRotator ReturnValue;
	};

	GetRotation_Params params;

	Globals::ProcessEvent(FindActorsFromClass(UObject::GetObjectFromName(XORSTRING("Class FortniteGame.FortAthenaAircraft")))[0], Functions::GetRotation, &params);

	return params.ReturnValue;
}

void SetActorRotation(FRotator Rot)
{
	struct SetRotation_Params
	{
		FRotator Rotation;
		bool bTeleportPhysx;
	};

	SetRotation_Params params;
	params.Rotation = Rot;
	params.bTeleportPhysx = false;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::SetRotation, &params);
}


void AdjustRotation()
{
	FRotator CurrentRotation = GetRotation();
	CurrentRotation.Pitch = 0;
	CurrentRotation.Roll = 0;

	SetActorRotation(CurrentRotation);
}

void CustomizationLoadout()
{
	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::ReplicateLoadout, &params);
}

void SetOwner(UObject* Target)
{
	struct SetOwner_Params
	{
		UObject* Owner;
	};

	SetOwner_Params params;
	params.Owner = Globals::PlayerController;


	Globals::ProcessEvent(Target, UObject::GetObjectFromName(XORSTRING("Function Engine.Actor.SetOwner")), &params);
}

UObject* CreateItem(UObject* ItemDefinition, int Count)
{
	struct CreateTemporaryItem_Params
	{
		int A;
		int B;
		UObject* ReturnValue;
	};

	struct SetController_Params
	{
		UObject* Controller;
	};

	SetController_Params SecondParams;
	SecondParams.Controller = Globals::PlayerController;

	CreateTemporaryItem_Params params;
	params.A = 1;
	params.B = 1; 

	Globals::ProcessEvent(ItemDefinition, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortItemDefinition.CreateTemporaryItemInstanceBP")), &params);
	auto FortItem = params.ReturnValue;

	Globals::ProcessEvent(FortItem, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortItem.SetOwningControllerForTemporaryItem")), &SecondParams);

	if (Count > 0)
	{
		reinterpret_cast<ItemCount*>(reinterpret_cast<uintptr_t>(FortItem) + OffsetTable::ItemEntry)->Count = Count;
	}

	return FortItem;
}

void UpdateInventory()
{
	struct EmptyParams
	{

	};

	EmptyParams params;

	Globals::ProcessEvent(Globals::PlayerController, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerController.HandleWorldInventoryLocalUpdate")), &params);
	Globals::ProcessEvent(Globals::Inventory, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortInventory.HandleInventoryLocalUpdate")), &params);
	Globals::ProcessEvent(Globals::PlayerController, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortPlayerController.OnRep_QuickBar")), &params);
	Globals::ProcessEvent(Globals::Quickbar, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortQuickBars.OnRep_SecondaryQuickBar")), &params);
	Globals::ProcessEvent(Globals::Quickbar, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortQuickBars.OnRep_PrimaryQuickBar")), &params);
}

FGuid GetGuid(UObject* FortItem)
{
	struct GetGuid_Params
	{
		FGuid ReturnValue;
	};

	GetGuid_Params params;
	
	Globals::ProcessEvent(FortItem, Functions::GetGuid, &params);

	return params.ReturnValue;
}

void AddToInventory(UObject* FortItem, EFortQuickBars QuickbarIndex, int Slot)
{

	struct ServerAddItem_Params
	{
		FGuid Item;
		EFortQuickBars Quickbar;
		int Slot;
	};


	reinterpret_cast<TArray<UObject*>*>(reinterpret_cast<uintptr_t>(Globals::Inventory) + OffsetTable::ItemInstances)->Add(FortItem);
	if (UniversalStructInstance->CurrentVersion < 3) { struct ItemEntrySize { char Size[0xB0]; }; reinterpret_cast<TArray<ItemEntrySize>*>(reinterpret_cast<uintptr_t>(Globals::Inventory) + OffsetTable::ItemEntries)->Add(*reinterpret_cast<ItemEntrySize*>(reinterpret_cast<uintptr_t>(FortItem) + OffsetTable::ItemEntry)); }
	else if (UniversalStructInstance->CurrentVersion < 5) { struct ItemEntrySize { char Size[0xC0]; }; reinterpret_cast<TArray<ItemEntrySize>*>(reinterpret_cast<uintptr_t>(Globals::Inventory) + OffsetTable::ItemEntries)->Add(*reinterpret_cast<ItemEntrySize*>(reinterpret_cast<uintptr_t>(FortItem) + OffsetTable::ItemEntry)); }

	ServerAddItem_Params params;
	params.Item = GetGuid(FortItem);
	params.Quickbar = QuickbarIndex;
	params.Slot = Slot;

	Globals::ProcessEvent(Globals::Quickbar, UObject::GetObjectFromName(XORSTRING("Function FortniteGame.FortQuickBars.ServerAddItemInternal")), &params);
}

UObject* GetDefinition(UObject* FortItem)
{
	struct GetItemDef_Params
	{
		UObject* ItemDefinition;
	};
	GetItemDef_Params params;

	Globals::ProcessEvent(FortItem, Functions::GetDefinition, &params);

	return params.ItemDefinition;
}

UObject* GetQuickbarItem(EFortQuickBars QuickBarIndex, int Slot)
{
	struct GetQuickbarItem_Params
	{
		EFortQuickBars QuickbarIndex;
		int Slot;
		UObject* ReturnValue;
	};

	GetQuickbarItem_Params params;
	params.QuickbarIndex = QuickBarIndex;
	params.Slot = Slot;

	Globals::ProcessEvent(Globals::InventoryContext, Functions::GetQuickbarItem, &params);

	return params.ReturnValue;
	
}

UObject* EquipWeapon(UObject* ItemDef, FGuid ItemGuid)
{
	struct EquipWeapon_Params
	{
		UObject* ItemDefinition;
		FGuid ItemGuid;
		UObject* ReturnValue;
	};

	EquipWeapon_Params params;
	params.ItemDefinition = ItemDef;
	params.ItemGuid = ItemGuid;

	Globals::ProcessEvent(Globals::PlayerPawn, Functions::EquipWeapon, &params);

	return params.ReturnValue;
}

void Inventory()
{
	struct InventoryPointer
	{
		UObject* Inventory;
	};

	struct QuickBarPointer
	{
		UObject* QuickBar;
	};

	Globals::Inventory = reinterpret_cast<InventoryPointer*>(reinterpret_cast<uintptr_t>(Globals::PlayerController) + OffsetTable::Inventory)->Inventory;
	Globals::Quickbar = SpawnActorFromClass(XORSTRING("Class FortniteGame.FortQuickBars"), GetLocation());
	reinterpret_cast<QuickBarPointer*>(reinterpret_cast<uintptr_t>(Globals::PlayerController) + OffsetTable::QuickBar)->QuickBar = Globals::Quickbar;

	SetOwner(Globals::Inventory);
	SetOwner(Globals::Quickbar);

	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01")), 0), EFortQuickBars::Primary, 0);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall")), 0), EFortQuickBars::Secondary, 0);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor")), 0), EFortQuickBars::Secondary, 1);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W")), 0), EFortQuickBars::Secondary, 2);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS")), 0), EFortQuickBars::Secondary, 3);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortResourceItemDefinition WoodItemData.WoodItemData")), 999), EFortQuickBars::Max_None, 0);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortResourceItemDefinition StoneItemData.StoneItemData")), 999), EFortQuickBars::Max_None, 0);
	AddToInventory(CreateItem(UObject::GetObjectFromName(XORSTRING("FortResourceItemDefinition MetalItemData.MetalItemData")), 999), EFortQuickBars::Max_None, 0);
	
	UpdateInventory();

}



#endif // !GAMEFUNCTIONS_H
