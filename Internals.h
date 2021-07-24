/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef INTERNALS_H
#define INTERNALS_H

#include "xorstr.hpp"
#define XORSTRING(STR) xorstr(STR).crypt_get()
#include <Windows.h>
#include <psapi.h>
#include "UnrealEngineStructs.h"
#include "OffsetTable.h"

BOOL MaskCompare(PVOID pBuffer, LPCSTR lpPattern, LPCSTR lpMask)
{
	for (auto value = static_cast<PBYTE>(pBuffer); *lpMask; ++lpPattern, ++lpMask, ++value)
	{
		if (*lpMask == 'x' && *reinterpret_cast<LPCBYTE>(lpPattern) != *value)
			return false;
	}

	return true;
}

PBYTE FindPattern(PVOID pBase, DWORD dwSize, LPCSTR lpPattern, LPCSTR lpMask)
{
	dwSize -= static_cast<DWORD>(strlen(lpMask));

	for (auto i = 0UL; i < dwSize; ++i)
	{
		auto pAddress = static_cast<PBYTE>(pBase) + i;

		if (MaskCompare(pAddress, lpPattern, lpMask))
			return pAddress;
	}

	return NULL;
}
PBYTE FindPattern(LPCSTR lpPattern, LPCSTR lpMask)
{
	MODULEINFO info = { 0 };

	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

	PBYTE result = FindPattern(info.lpBaseOfDll, info.SizeOfImage, lpPattern, lpMask);

	return result;
}

namespace EngineVersion
{
	void InitEngineVersion()
	{
		UniversalStructInstance->addr_GetEngineVersion = FindPattern(XORSTRING("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\xC8\x41\xB8\x04\x00\x00\x00\x48\x8B\xD3"), XORSTRING("xxxxxxxxxx????xxxxxxxxxxxx"));
	}

	string GetVersion(void* Result)
	{
		InitEngineVersion();
		auto ToConvert = reinterpret_cast<FString * (__fastcall*)(void*)>(UniversalStructInstance->addr_GetEngineVersion)(Result);
		string Converted;
		Converted = ToConvert->ToString();
		return Converted;
	}
}


#endif // !INTERNALS_H