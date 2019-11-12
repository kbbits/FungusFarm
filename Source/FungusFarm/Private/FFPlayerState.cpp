// Fill out your copyright notice in the Description page of Project Settings.


#include "FFPlayerState.h"
#include "FFGameMode.h"

AFFPlayerState::AFFPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerProperties.ExperienceLevel = 1;
	PlayerProperties.ExperiencePoints = 0.0f;
}


float AFFPlayerState::GetExperience()
{
	return PlayerProperties.ExperiencePoints;
}


// Returns the new total experience
float AFFPlayerState::AddExperience(const float Experience)
{
	if (Experience == 0.0f)
	{
		return GetExperience();
	}

	float OldXp = GetExperience();
	PlayerProperties.ExperiencePoints = FMath::Max<float>(Experience + PlayerProperties.ExperiencePoints, 0.0f);
	if (OldXp != GetExperience())
	{
		OnExperienceChanged.Broadcast(PlayerProperties.ExperiencePoints);
		CheckForLevelUp();
	}
	return GetExperience();
}


void AFFPlayerState::SetExperience(const float Experience)
{
	float OldXp = GetExperience();
	PlayerProperties.ExperiencePoints = FMath::Max<float>(Experience, 0.0f);
	if (OldXp != GetExperience())
	{
		OnExperienceChanged.Broadcast(PlayerProperties.ExperiencePoints);
		CheckForLevelUp();
	}
}


int32 AFFPlayerState::GetExperienceLevel()
{
	return PlayerProperties.ExperienceLevel;
}


void AFFPlayerState::SetExperienceLevel(const int32 NewLevel)
{
	float OldLevel = GetExperienceLevel();
	PlayerProperties.ExperienceLevel = FMath::Max<int32>(NewLevel, 0);
	if (OldLevel != GetExperienceLevel())
	{
		OnExperienceLevelChanged.Broadcast(GetExperienceLevel());
	}
}


bool AFFPlayerState::CheckForLevelUp()
{
	UWorld* _World = GetWorld();
	if (_World)
	{
		AFFGameMode * _GameMode = Cast<AFFGameMode>(_World->GetAuthGameMode());
		if (_GameMode)
		{
			float XpRequired = _GameMode->GetExperienceRequiredForLevel(GetExperienceLevel() + 1);
			UE_LOG(LogFFGame, Verbose, TEXT("Level %d XP required: %f current: %f"), GetExperienceLevel() + 1, XpRequired, GetExperience());
			if (GetExperience() >= XpRequired)
			{
				SetExperienceLevel(GetExperienceLevel() + 1);
			    UE_LOG(LogFFGame, Verbose, TEXT("Experience leveled went up to %d"), GetExperienceLevel());
				return true;
			}
		}
		else
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s CheckForLevelUp could not get GameMode."), *GetNameSafe(this));
		}
	}
	return false;
}
