// Fill out your copyright notice in the Description page of Project Settings.


#include "FFDefaultSavePreset.h"


UFFDefaultSavePreset::UFFDefaultSavePreset() : Super()
{
	bSaveOnExit = false;
	bAutoLoad = false;
	MultithreadedSerialization = ESaveASyncMode::OnlySync;
	FrameSplittedSerialization = ESaveASyncMode::OnlySync;
	MultithreadedFiles = ESaveASyncMode::OnlySync;
}