// Copyright Epic Games, Inc. All Rights Reserved.

#include "BethesdaInternshipPlayerController.h"

#include <string>

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "BethesdaInternshipCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Grid.h"
#include "Tile.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

ABethesdaInternshipPlayerController::ABethesdaInternshipPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void ABethesdaInternshipPlayerController::BeginPlay()
{
	ControlledPawn = Cast<ABethesdaInternshipCharacter>(GetPawn());
	GameMode = Cast<ABethesdaInternshipGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	TileGridArray = IGridInterface::Execute_GetTileGridArray(GameMode->Grid);
}

void ABethesdaInternshipPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();		
	}

	ATile* HitTile = ControlledPawn->HitTile;
	if((HitTile != nullptr))
	{
		if (HitTile->bInRange)
		{
			ControlledPawn->HitTile->TileMaterialInstance->SetVectorParameterValue(FName("HoverColor"), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			ControlledPawn->HitTile->TileMaterialInstance->SetVectorParameterValue(FName("HoverColor"), FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		}
		ControlledPawn->HitTile->TileMaterialInstance->SetScalarParameterValue(FName("Hovered"), 10.f);
		ControlledPawn->HitTile->StartHovering();
	}

	for (ATile* Tile : TileGridArray)
	{
		float DistanceFromPlayer = FVector::Distance(ControlledPawn->GetActorLocation(), Tile->GetActorLocation());
		if (UKismetMathLibrary::Abs(DistanceFromPlayer) <= 444.f)
		{
			Tile->SetTileColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			Tile->bInRange = true;
		}
		else
		{
			Tile->SetTileColor(FLinearColor(0.f, 0.f, 0.f, 1.f));
			Tile->bInRange = false;
		}
	}

	if (bMoving)
	{
		MoveToTile(NextTile);
		if (FVector::DistXY(ControlledPawn->GetActorLocation(), NextTile->GetActorLocation()) < 20.f)
		{
			if (NextTile != GoalTile)
			{
				NextTile = NextTile->Child;
			}
			else
			{
				ControlledPawn->CurrentTile = GoalTile;
				bMoving = false;
			}
			NextTile->bIsOccupiedByPawn = true;
		}
	}
	
}

void ABethesdaInternshipPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ABethesdaInternshipPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ABethesdaInternshipPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABethesdaInternshipPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABethesdaInternshipPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ABethesdaInternshipPlayerController::OnResetVR);
}

void ABethesdaInternshipPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABethesdaInternshipPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ABethesdaInternshipCharacter* MyPawn = Cast<ABethesdaInternshipCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ABethesdaInternshipPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ABethesdaInternshipPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ABethesdaInternshipPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	auto HitTile = ControlledPawn->HitTile;
	if (HitTile && !bMoving)
	{
		if(UGameplayStatics::GetGameMode(GetWorld()) == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Grid not found..."))
			return;
		}

		if (!ensure(GameMode!=nullptr))
			return;

		int Index =  IGridInterface::Execute_GetTileGridArray(GameMode->Grid).Find(HitTile);
		//FVector2D Coord =  GameMode->Grid->IndexToCoordinate(Index);
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Yellow, FString::Printf(TEXT("Coordinate is Pos: %i, %i "), HitTile->GetCoordinates().X, HitTile->GetCoordinates().Y), true, FVector2D(3.f));
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Yellow, FString::Printf(TEXT("Index is %i"), GameMode->Grid->CoordinateToIndex(FIntPoint(HitTile->PositionX, HitTile->PositionY))), true, FVector2D(3.f));


		//UE_LOG(LogTemp, Warning, TEXT("Current TIle is %i, %i"), ControlledPawn->CurrentTile->GetCoordinates().X, ControlledPawn->CurrentTile->GetCoordinates().Y);
		//UE_LOG(LogTemp, Warning, TEXT("Hit TIle is %i, %i"), HitTile->GetCoordinates().X, HitTile->GetCoordinates().Y);


			// Searching Path Finding
		if (HitTile->bInRange)
		{
			for (ATile* Tile : TileGridArray)
			{
				Tile->bVisited = false;
				Tile->Parent = nullptr;
			}
			
			bool Success = GameMode->Grid->FindPath(ControlledPawn->CurrentTile, HitTile);
			GameMode->Grid->SetChild(HitTile);

			GoalTile = HitTile;
			NextTile = ControlledPawn->CurrentTile->Child;
			bMoving = true;

			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Yellow, FString::Printf(TEXT("BFS Algorithm Result %i"), Success), true, FVector2D(3.f));
		}

			
			//UE_LOG(LogTemp, Warning, TEXT("Index = %i, Coordinate = (%i, %i) "), Index, HitTile->PositionX, HitTile->PositionY);
			// UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitTile->GetActorLocation());
	}
}

void ABethesdaInternshipPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
}

void ABethesdaInternshipPlayerController::MoveToTile(ATile* Tile)
{
	if (ControlledPawn->CurrentTile != NextTile)
	{
		ControlledPawn->SetActorLocation(FMath::Lerp(ControlledPawn->GetActorLocation(), Tile->GetActorLocation(), 0.07f));
	}
}
