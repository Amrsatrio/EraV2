/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef OFFSETINIT_H
#define OFFSETINIT_H

#include "Internals.h"
#include "OffsetTable.h"

namespace EraInit
{

	void Console()
	{
		AllocConsole();

		ShowWindow(GetConsoleWindow(), SW_SHOW);
		FILE* fp;
		freopen_s(&fp, "CONOIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);

		printf(XORSTRING("Era 1.3 || Rewritten.\nMade by danii#4000 (DLL)\nBackend by Kyiro#7884\nLauncher by ozne#3303 and Not a Robot#6932\nEnjoy!\n"));
	}

	void SetOffsets(string EngineVersion)
	{
		if (EngineVersion.find(XORSTRING("3807424")) != string::npos)
		{
			UniversalStructInstance->CurrentVersion = 1;
			GObjectsPointer = OffsetTable::GObjects = reinterpret_cast<FUObjectArray*>((PBYTE)base_address + 0x5CC1310);
			GetObjectNameOffset = OffsetTable::GetObjectName = 0x1A51920;
			OffsetTable::ProcessEvent = FindPattern(XORSTRING("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C"), XORSTRING("xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx"));
			OffsetTable::UEngine = 0x5DA3F20;
			OffsetTable::ViewPort = 0x720;
			OffsetTable::World = 0x80;
			OffsetTable::GameInstance = 0x88;
			OffsetTable::LocalPlayers = 0x38;
			OffsetTable::PlayerController = 0x30;
			OffsetTable::GameMode = 0xF0;
			OffsetTable::GameState = 0xF8;
			OffsetTable::MinimapBrush = 0x1438;
			OffsetTable::MinimapSecondBrush = 0x0;
			OffsetTable::StrongMyHero = 0x31B0;
			OffsetTable::CharacterParts = 0x280;
			OffsetTable::AdditionalData = 0x530;
			OffsetTable::SprintingBool = 0xF85;
			OffsetTable::MovementStyle = 0x804;
			OffsetTable::MinimapThirdBrush = 0x0;
			OffsetTable::MinimapFourthBrush = 0x0;
			OffsetTable::MinimapFifthBrush = 0x0;
			OffsetTable::MinimapSixthBrush = 0x0;
			OffsetTable::MinimapSeventhBrush = 0x0;
			OffsetTable::Inventory = 0x1DB8;
			OffsetTable::QuickBar = 0x1B10;
			OffsetTable::ItemInstances = 0x488;
			OffsetTable::ItemEntries = 0x428;
			OffsetTable::ItemEntry = 0x1E0;
			OffsetTable::SpawnActor = FindPattern(XORSTRING("\xE8\x00\x00\x00\x00\x0F\x10\x04\x37"), XORSTRING("x????xxxx"));
		}
		else if (EngineVersion.find(XORSTRING("3870737")) != string::npos)
		{
			UniversalStructInstance->CurrentVersion = 2;
			GObjectsPointer = OffsetTable::GObjects = reinterpret_cast<FUObjectArray*>((PBYTE)base_address + 0x44E5CE0);
			GetObjectNameOffset = OffsetTable::GetObjectName = 0x1A8B7D0;
			OffsetTable::ProcessEvent = FindPattern(XORSTRING("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C"), XORSTRING("xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx"));
			OffsetTable::UEngine = 0x45CB8A0;
			OffsetTable::ViewPort = 0x720;
			OffsetTable::World = 0x80;
			OffsetTable::GameInstance = 0x88;
			OffsetTable::LocalPlayers = 0x38;
			OffsetTable::PlayerController = 0x30;
			OffsetTable::GameMode = 0x140;
			OffsetTable::GameState = 0x148;
			OffsetTable::MinimapBrush = 0x1458;
			OffsetTable::MinimapSecondBrush = 0x14E0;
			OffsetTable::StrongMyHero = 0x2EB0;
			OffsetTable::CharacterParts = 0x280;
			OffsetTable::AdditionalData = 0x4F0;
			OffsetTable::SprintingBool = 0x1015;
			OffsetTable::MovementStyle = 0x814;
			OffsetTable::MinimapThirdBrush = 0x0;
			OffsetTable::MinimapFourthBrush = 0x0;
			OffsetTable::MinimapFifthBrush = 0x0;
			OffsetTable::MinimapSixthBrush = 0x0;
			OffsetTable::MinimapSeventhBrush = 0x0;
			OffsetTable::Inventory = 0x1E18;
			OffsetTable::QuickBar = 0x1B70;
			OffsetTable::ItemInstances = 0x488;
			OffsetTable::ItemEntries = 0x428;
			OffsetTable::ItemEntry = 0x1E0;
			OffsetTable::SpawnActor = FindPattern(XORSTRING("\xE8\x00\x00\x00\x00\x0F\x10\x04\x3E"), XORSTRING("x????xxxx"));
		}
		else if (EngineVersion.find(XORSTRING("3935073")) != string::npos)
		{
			UniversalStructInstance->CurrentVersion = 3;
			GObjectsPointer = OffsetTable::GObjects = reinterpret_cast<FUObjectArray*>((PBYTE)base_address + 0x48F72F0);
			GetObjectNameOffset = OffsetTable::GetObjectName = 0x210CAC0;
			OffsetTable::ProcessEvent = FindPattern(XORSTRING("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C"), XORSTRING("xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx"));
			OffsetTable::UEngine = 0x49EF310;
			OffsetTable::ViewPort = 0x720;
			OffsetTable::World = 0x88;
			OffsetTable::GameInstance = 0x90;
			OffsetTable::LocalPlayers = 0x38;
			OffsetTable::PlayerController = 0x30;
			OffsetTable::GameMode = 0x140;
			OffsetTable::GameState = 0x148;
			OffsetTable::MinimapBrush = 0x15B8;
			OffsetTable::MinimapSecondBrush = 0x1660;
			OffsetTable::StrongMyHero = 0x2FC0;
			OffsetTable::CharacterParts = 0x258;
			OffsetTable::AdditionalData = 0x500;
			OffsetTable::SprintingBool = 0xFE5;
			OffsetTable::MovementStyle = 0x7C4;
			OffsetTable::MinimapThirdBrush = 0x16D8;
			OffsetTable::MinimapFourthBrush = 0x1750;
			OffsetTable::MinimapFifthBrush = 0x17C8;
			OffsetTable::MinimapSixthBrush = 0x1840;
			OffsetTable::MinimapSeventhBrush = 0x18B8;
			OffsetTable::Inventory = 0x1DE8;
			OffsetTable::QuickBar = 0x1B48;
			OffsetTable::ItemInstances = 0x438;
			OffsetTable::ItemEntries = 0x3D8;
			OffsetTable::ItemEntry = 0x1E0;
			OffsetTable::SpawnActor = FindPattern(XORSTRING("\xE8\x00\x00\x00\x00\x0F\x10\x04\x3E"), XORSTRING("x????xxxx"));
		}
	}
}


#endif // !OFFSETINIT_H