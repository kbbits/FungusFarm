// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGoalTemplate.h"

bool FGameplayGoalTemplate::operator== (FGameplayGoalTemplate& other)
{
	return (UniqueNameBase == other.UniqueNameBase);
}

bool FGameplayGoalTemplate::operator== (const FGameplayGoalTemplate& other)
{
	return (UniqueNameBase == other.UniqueNameBase);
}

bool FGameplayGoalTemplate::operator== (const FGameplayGoalTemplate& other) const
{
	return (UniqueNameBase == other.UniqueNameBase);
}

bool FGameplayGoalTemplate::operator==(const FName & otherName) const
{
	return (UniqueNameBase == otherName);
}

bool FGameplayGoalTemplate::operator==(const FName & otherName)
{
	return (UniqueNameBase == otherName);
}

bool FGameplayGoalTemplate::operator==(FName & otherName)
{
	return (UniqueNameBase == otherName);
}