// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BethesdaInternshipGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GridInterface.h"
#include "BethesdaInternshipPlayerController.generated.h"

class AGrid;
class ABethesdaInternshipCharacter;

UCLASS()
class ABethesdaInternshipPlayerController : public APlayerController, public IGridInterface
{
	GENERATED_BODY()

public:
	ABethesdaInternshipPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AGrid> GridBlueprint;

	ABethesdaInternshipGameMode* GameMode;

	ATile* HoveredTile;

	ABethesdaInternshipCharacter* ControlledPawn;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void MoveToTile(ATile* Tile);

private:
	TArray<ATile*> TileGridArray;
	bool bMoving = false;
	ATile* NextTile;
	ATile* GoalTile;
	
};


