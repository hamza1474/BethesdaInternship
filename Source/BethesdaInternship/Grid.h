// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "BethesdaInternshipGameMode.h"
#include "GridInterface.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class ATile;
class ADoorTile;
class AFloorTile;

UCLASS()
class BETHESDAINTERNSHIP_API AGrid : public AActor, public IGridInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual TArray<ATile*> GetTileGridArray_Implementation() override;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateGrid();

	ABethesdaInternshipGameMode* GameMode;


	// Default Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeHorizontal = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GridSizeVertical = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GridOffset = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AFloorTile> FloorTileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ADoorTile> DoorTileBlueprint;

	// Main Array
	TArray<ATile*> TileGridArray;

	// Pathfinder Functions

	bool FindPath(ATile* StartTile, ATile* EndTile);
	void ColorPath(ATile* EndTile);
	void SetChild(ATile* EndTile);


	// Info
	FIntPoint IndexToCoordinate(int Index);
	int CoordinateToIndex(FIntPoint Coord);

	bool IsValidCoordinate(FIntPoint Coord);
	ATile* GetTileAtCoordinate(FIntPoint Coord);

	ATile* BFS(FIntPoint CurrentPosition, FIntPoint GoalPosition);

	void SetTileAreaColor(ATile* Center, int Area);
};


