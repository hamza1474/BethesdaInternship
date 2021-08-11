// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


UENUM()
enum ETileType
{
	Open,
    Blocked,
    Door,
	Floor
};

UCLASS()
class BETHESDAINTERNSHIP_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnHoverStart();
	void OnHoverEnd();
	


public:
	
	int PositionX;
	int PositionY;
	
	ATile* Parent;
	ATile* Child;

	bool bIsHovered = false;
	bool bHoverFade = false;

	void StartHovering();
	void EndHovering();

	void SetTileColor(FLinearColor Color);
	
	UFUNCTION(BlueprintCallable)
	FIntPoint GetCoordinates();

	bool bVisited = false;
	bool bInRange = false;
	
	bool bIsOccupiedByPawn;

	ETileType TileType = ETileType::Open;


public:
	UMaterial* TileMaterialAsset;
	USceneComponent* SceneRoot;
	UStaticMeshComponent* TileMesh;
	UMaterialInstanceDynamic* TileMaterialInstance;

	FTimerDelegate HoverTimerDel;
	FTimerHandle HoverTimerHandle;

	UFUNCTION()
    void OnHoverTimerEnd();


};
