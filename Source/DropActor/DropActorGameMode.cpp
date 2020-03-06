// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "DropActorGameMode.h"
#include "DropActorHUD.h"
#include "DropActorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADropActorGameMode::ADropActorGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADropActorHUD::StaticClass();
}
