// Copyright Epic Games, Inc. All Rights Reserved.

#include "AgronomTestGameMode.h"
#include "AgronomTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAgronomTestGameMode::AAgronomTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
