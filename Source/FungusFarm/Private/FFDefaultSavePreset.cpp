// Fill out your copyright notice in the Description page of Project Settings.


#include "FFDefaultSavePreset.h"
#include "FFSlotInfo.h"

UFFDefaultSavePreset::UFFDefaultSavePreset() : Super()
{
	SlotInfoTemplate = UFFSlotInfo::StaticClass();
}