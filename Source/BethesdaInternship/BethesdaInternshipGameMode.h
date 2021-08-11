// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GridInterface.h"
#include "GameFramework/GameModeBase.h"
#include "BethesdaInternshipGameMode.generated.h"

class ATile;
class AGrid;

UCLASS(minimalapi)
class ABethesdaInternshipGameMode : public AGameModeBase, public IGridInterface
{
	GENERATED_BODY()

public:
	ABethesdaInternshipGameMode();

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Grid)
	TSubclassOf<AGrid> GridBlueprint;

	UPROPERTY(EditAnywhere)
	int GridSizeHorizontal = 8;

	UPROPERTY(EditAnywhere)
	int GridSizeVertical = 8;

	AGrid* Grid;


};



