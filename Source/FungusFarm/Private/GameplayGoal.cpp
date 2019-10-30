// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGoal.h"

bool FGameplayGoal::operator== (FGameplayGoal& other)
{
	return (UniqueName == other.UniqueName);
}

bool FGameplayGoal::operator== (const FGameplayGoal& other)
{
	return (UniqueName == other.UniqueName);
}

bool FGameplayGoal::operator== (const FGameplayGoal& other) const
{
	return (UniqueName == other.UniqueName);
}