/*EraFN Copyright (C) 2021 Daniele "danii" Giompaolo

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#ifndef UE4_H
#define UE4_H

#include <Windows.h>
#include <string>
#include <locale>
#include <set>
#include <fstream>

int GetObjectNameOffset = 0;
int EngineOffset = 0;

static __forceinline bool IsBadReadPtr(void* p)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(p, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

		return b;
	}
	return true;
}


using namespace std;

class UObject;

class FNameEntry
{
public:
	static const auto NAME_WIDE_MASK = 0x1;
	static const auto NAME_INDEX_SHIFT = 1;

	int32_t Index;
	char UnknownData00[0x04];
	FNameEntry* HashNext;
	union
	{
		char AnsiName[1024];
		wchar_t WideName[1024];
	};

};

template<typename ElementType, int32_t MaxTotalElements, int32_t ElementsPerChunk>
class TStaticIndirectArrayThreadSafeRead
{
public:
	inline size_t Num() const
	{
		return NumElements;
	}

	inline bool IsValidIndex(int32_t index) const
	{
		return index < Num() && index >= 0;
	}

	inline ElementType const* const& operator[](int32_t index) const
	{
		return *GetItemPtr(index);
	}

private:
	inline ElementType const* const* GetItemPtr(int32_t Index) const
	{
		int32_t ChunkIndex = Index / ElementsPerChunk;
		int32_t WithinChunkIndex = Index % ElementsPerChunk;
		ElementType** Chunk = Chunks[ChunkIndex];
		return Chunk + WithinChunkIndex;
	}

	enum
	{
		ChunkTableSize = (MaxTotalElements + ElementsPerChunk - 1) / ElementsPerChunk
	};

	ElementType** Chunks[ChunkTableSize];
	int32_t NumElements;
	int32_t NumChunks;
};

using TNameEntryArray = TStaticIndirectArrayThreadSafeRead<FNameEntry, 2 * 1024 * 1024, 16384>;

struct FName
{
	union
	{
		struct
		{
			int32_t ComparisonIndex;
			int32_t Number;
		};

		uint64_t CompositeComparisonValue;
	};

	static TNameEntryArray* GNames;
};

class FUObjectItem
{
public:
	UObject* Object;
	int32_t Flags;
	int32_t ClusterIndex;
	int32_t SerialNumber;

	enum class ObjectFlags : int32_t
	{
		None = 0,
		Native = 1 << 25,
		Async = 1 << 26,
		AsyncLoading = 1 << 27,
		Unreachable = 1 << 28,
		PendingKill = 1 << 29,
		RootSet = 1 << 30,
		NoStrongReference = 1 << 31
	};

	inline bool IsUnreachable() const
	{
		return !!(Flags & static_cast<std::underlying_type_t<ObjectFlags>>(ObjectFlags::Unreachable));
	}
	inline bool IsPendingKill() const
	{
		return !!(Flags & static_cast<std::underlying_type_t<ObjectFlags>>(ObjectFlags::PendingKill));
	}
};

class TUObjectArray
{
public:
	inline int32_t Num() const
	{
		return NumElements;
	}

	inline UObject* GetByIndex(int32_t index) const
	{
		return Objects[index].Object;
	}

	inline FUObjectItem* GetItemByIndex(int32_t index) const
	{
		if (index < NumElements)
		{
			return &Objects[index];
		}
		return nullptr;
	}

	FUObjectItem* Objects;
	int32_t MaxElements;
	int32_t NumElements;

};

class FUObjectArray
{
public:
	TUObjectArray ObjObjects;
};

template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

	void Add(T InputData)
	{
		Data = (T*)realloc(Data, sizeof(T) * (Count + 1));
		Data[Count++] = InputData;
		Max = Count;
	};

	T* Data;
	int32_t Count;
	int32_t Max;

private:

};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

FUObjectArray* GObjectsPointer;

class UObject
{
public:
	static FUObjectArray* GObjects;
	void* Vtable;
	int32_t ObjectFlags;
	int32_t InternalIndex;
	class UObject* Class;
	FName Name;
	UObject* Outer;

	bool IsA(UObject* cmp)
	{
		if (Class == cmp)
		{
			return true;
		}
		return false;
	}

	string GetObjectNameString()
	{

		auto Object = reinterpret_cast<uintptr_t>(this);
		if (Object == NULL) return (char*)"";

		uintptr_t base_address = (uintptr_t)GetModuleHandle(NULL);
		auto GetObjName = reinterpret_cast<FString * (__fastcall*)(FString*, uintptr_t)>((base_address + GetObjectNameOffset));

		FString result;
		GetObjName(&result, Object);

		if (result.c_str() == NULL) return (char*)"";

		auto tmp = result.ToString();
		if (tmp == "") return (char*)"";

		if (Name.Number > 0)
		{
			tmp += '_' + std::to_string(Name.Number);
		}

		auto pos = tmp.rfind('/');
		if (pos == std::string::npos)
		{
			return tmp;
		}

		return tmp.substr(pos + 1);

	}

	string GetFullName()
	{
		std::string temp;

		for (auto outer = this->Outer; outer; outer = outer->Outer)
		{
			temp = outer->GetObjectNameString() + "." + temp;
		}

		temp = reinterpret_cast<UObject*>(Class)->GetObjectNameString() + " " + temp + this->GetObjectNameString();

		return temp;
	}



	static UObject* GetObjectFromName(string ObjectName)
	{
		for (auto i = 0; i < GObjectsPointer->ObjObjects.NumElements; i++)
		{
			UObject* CurrentObject = GObjectsPointer->ObjObjects.GetByIndex(i);

			if (CurrentObject == nullptr)
			{
				continue;
			}

			auto CurObjectNameString = CurrentObject->GetFullName();

			if (CurObjectNameString == ObjectName)
			{
				return CurrentObject;
			}
		}
	}

	static UObject* FindPartOfObject(string ObjectName)
	{
		for (auto i = 0; i < GObjectsPointer->ObjObjects.NumElements; i++)
		{
			UObject* CurrentObject = GObjectsPointer->ObjObjects.GetByIndex(i);

			if (CurrentObject == nullptr)
			{
				continue;
			}

			auto CurObjectNameString = CurrentObject->GetFullName();

			if (CurObjectNameString.find(ObjectName) != string::npos)
			{
				return CurrentObject;
			}
		}
	}

	static void DumpObjects()
	{
		ofstream DumpFile;

		DumpFile.open("GObjects.txt");

		for (auto i = 0; i < GObjectsPointer->ObjObjects.NumElements; i++)
		{
			UObject* CurrentObject = GObjectsPointer->ObjObjects.GetByIndex(i);

			if (CurrentObject == nullptr)
			{
				continue;
			}

			auto CurObjectNameString = CurrentObject->GetFullName();

			
			DumpFile << CurObjectNameString << "\n";
			
		}

		DumpFile.close();
	}
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};

struct FVector
{
	float X;
	float Y;
	float Z;
};

struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;
};

struct FVector2D
{
	float X;            
	float Y; 
};

struct FMargin
{
	float Left;     
	float Top;      
	float Right;    
	float Bottom;   
};

enum class ESlateColorStylingMode : uint8_t
{
	UseColor_Specified = 0,
	UseColor_Specified_Link = 1,
	UseColor_Foreground = 2,
	UseColor_Foreground_Subdued = 3,
	UseColor_MAX = 4
};

struct FLinearColor
{
	float R;         
	float G;         
	float B;         
	float A;         
};

struct FSlateColor
{
	FLinearColor SpecifiedColor;        
	TEnumAsByte<ESlateColorStylingMode> ColorUseRule;          
	unsigned char UnknownData00[0x17];   
};

struct FSlateBrush
{
	unsigned char UnknownData00[0x8];                   
	FVector2D ImageSize;                            
	FMargin Margin;                               
	FSlateColor TintColor;                            
	UObject* ObjectResource;
};

enum class EFortCustomPartType : uint8_t
{
	Head = 0,
	Body = 1,
	Hat = 2,
	Backpack = 3,
	Charm = 4,
	Face = 5,
	NumTypes = 6,
	EFortCustomPartType_MAX = 7
};

enum class EFortMovementStyle : uint8_t
{
	Running = 0,
	Walking = 1,
	Charging = 2,
	Sprinting = 3,
	Hoverboard = 4,
	EFortMovementStyle_MAX = 5
};

enum class EFortQuickBars : uint8_t
{
	Primary = 0,
	Secondary = 1,
	Max_None = 2,
	EFortQuickBars_MAX = 3
};

struct FGuid
{
	int A;       
	int B;       
	int C;       
	int D;       
};

struct ItemCount
{
	char Bytes[0xC];
	int Count;
};

struct SlotFocusParams
{
	EFortQuickBars QuickbarIndex;
	int Slot;
};

#endif // !UE4_H