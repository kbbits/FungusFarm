// Fill out your copyright notice in the Description page of Project Settings.

#include "FFStaticLibrary.h"

UFFStaticLibrary::UFFStaticLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

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
