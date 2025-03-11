// Copyright Epic Games, Inc. All Rights Reserved.

#include "ue5dojostarterGameMode.h"
#include "ue5dojostarterCharacter.h"
#include "UObject/ConstructorHelpers.h"

Aue5dojostarterGameMode::Aue5dojostarterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
