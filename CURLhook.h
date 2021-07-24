/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef CURLHOOK_H
#define CURLHOOK_H
#include <Windows.h>
#include "LeoSpecial.h"
#include "Internals.h"
#include "OffsetTable.h"
#include "curl.h"
#include <regex>
#include "minhook/MinHook.h"
#pragma comment(lib, "minhook/libMinHook.x64.lib")
#include "ProcessEvent.h"
#include "Functions.h"

namespace CURLhook
{
	bool bHookRestarted = false;
	bool bInitGlobals = false;
	bool bMatchmaking = false;
	GameManager ClassInstance;
	void RestartHook();

	namespace Hooks
	{


		CURLcode(*CurlSet)(struct Curl_easy*, CURLoption, va_list) = nullptr;
		CURLcode CurlSetHook2(struct Curl_easy* A, CURLoption B, ...)
		{
		}

		CURLcode(*CurlEasy)(struct Curl_easy*, CURLoption, ...) = nullptr;
		void CurlEasyHook2(struct Curl_easy* A, CURLoption B, ...)
		{

			//TO DO :
			//Call InitializeGlobals on ClientQuestLogin
			//Call OpenAthenaTerrain on matchmakingservice 
			//Call RestartHook on oauth/token


			//Temporary
			return;
		}


	}

	void RestartHook()
	{
		UniversalStructInstance->CURLhook.Unhook();
		MH_Initialize();
		MH_CreateHook(static_cast<LPVOID>(UniversalStructInstance->CurlPattern), Hooks::CurlEasyHook2, reinterpret_cast<LPVOID*>(&Hooks::CurlEasy));
		MH_EnableHook(static_cast<LPVOID>(UniversalStructInstance->CurlPattern));

		InitializePEH();
	}

	void StartHook()
	{
		UniversalStructInstance->CurlPattern = FindPattern(XORSTRING("\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x48\x83\xEC\x28\x48\x85\xC9\x75\x08\x8D\x41\x2B\x48\x83\xC4\x28\xC3\x4C"), XORSTRING("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
		UniversalStructInstance->CurlPatternSet = FindPattern(XORSTRING("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x33\xED\x49\x8B\xF0\x48\x8B\xD9"), XORSTRING("xxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
		
		Hooks::CurlEasy = decltype(Hooks::CurlEasy)(UniversalStructInstance->CurlPattern);
		Hooks::CurlSet = decltype(Hooks::CurlSet)(UniversalStructInstance->CurlPatternSet);

		UniversalStructInstance->CURLhook.Hook((uintptr_t)Hooks::CurlEasy, (uintptr_t)Hooks::CurlEasyHook2);
	}

}

#endif // !CURLHOOK_H

