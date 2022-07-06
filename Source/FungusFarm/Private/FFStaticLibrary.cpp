// Fill out your copyright notice in the Description page of Project Settings.

#include "FFStaticLibrary.h"
#include "Misc/DefaultValueHelper.h"

bool FFFindSlotVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (bIsDirectory)
	{
		return true;
	}

	const FString FullFilePath(FilenameOrDirectory);

	FString Folder;
	FString Filename;
	FString Extension;
	FPaths::Split(FullFilePath, Folder, Filename, Extension);
	FString IdString;
	FString ProfileToken;
	int32 Id;
	// Profile filenames have the format:  <slot_id>_profile.sav
	if (Filename.Split(TEXT("_"), &IdString, &ProfileToken, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		if (ProfileToken == TEXT("profile"))
		{
			if (FDefaultValueHelper::ParseInt(IdString, Id))
			{
				FoundProfiles.Add(Filename);
				FoundSlotIds.Add(Id);
			}			
		}
	}
	return true;
}


void UFFStaticLibrary::SumModifiers(const TArray<FModifier>& ModsToSum, float& ScalarTotal, float& MultiplierTotal)
{
	ScalarTotal = 0;
	MultiplierTotal = 0;

	if (ModsToSum.Num() == 0)
		return;

	for (FModifier Mod : ModsToSum)
	{
		switch (Mod.Type)
		{
		case EModifierType::Multiplier:
			MultiplierTotal += Mod.Value;
			break;
		case EModifierType::Scalar:
			ScalarTotal += Mod.Value;
			break;
		}
	}
}

void UFFStaticLibrary::CompareStrings(const FString first, const FString second, int32 & relation, bool CaseSensitive)
{
	if (CaseSensitive) 
	{
		relation = first.Compare(second, ESearchCase::CaseSensitive);
	}
	else
	{
		relation = first.Compare(second, ESearchCase::IgnoreCase);
	}
}

void UFFStaticLibrary::FindSaveProfileFileNames(TArray<FString>& FoundProfiles, TArray<int32>& FoundSlotIds)
{
	FFFindSlotVisitor Visitor{ FoundProfiles, FoundSlotIds };
	FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*FString::Printf(TEXT("%sSaveGames/"), *FPaths::ProjectSavedDir()), Visitor);
}