// Copyright Epic Games, Inc. All Rights Reserved.

#include "BethesdaInternshipGameMode.h"
#include "BethesdaInternshipPlayerController.h"
#include "BethesdaInternshipCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Grid.h"

ABethesdaInternshipGameMode::ABethesdaInternshipGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABethesdaInternshipPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABethesdaInternshipGameMode::BeginPlay()
{
	Grid = GetWorld()->SpawnActor<AGrid>(GridBlueprint, FVector(0), FRotator(0));
	Grid->GridSizeHorizontal = GridSizeHorizontal;
	Grid->GridSizeVertical = GridSizeVertical;
	Grid->GenerateGrid();
	
}


