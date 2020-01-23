// Fill out your copyright notice in the Description page of Project Settings.

#include "NameCountInt.h"

bool FNameCountInt::operator== (FNameCountInt& other)
{
	return (Name == other.Name);
}

bool FNameCountInt::operator== (const FNameCountInt& other)
{
	return (Name == other.Name);
}

bool FNameCountInt::operator== (const FNameCountInt& other) const
{
	return (Name == other.Name);
}

bool FNameCountInt::operator==(const FName & otherName) const
{
	return (Name == otherName);
}

bool FNameCountInt::operator==(const FName & otherName)
{
	return (Name == otherName);
}

bool FNameCountInt::operator==(FName & otherName)
{
	return (Name == otherName);
}
