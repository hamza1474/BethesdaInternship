// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "DoorTile.generated.h"

/**
 * 
 */
UCLASS()
class BETHESDAINTERNSHIP_API ADoorTile : public ATile
{
	GENERATED_BODY()

	ADoorTile();
	
public:
	bool bDoorHorizontal;
	FIntPoint EntranceA = (-1, -1);
	FIntPoint EntranceB = (-1, -1);

	bool bIsOpen = false;

	UStaticMeshComponent* DoorMesh;
};
