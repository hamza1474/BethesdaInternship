// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TileMeshAssetRef(TEXT("StaticMesh'/Game/BethesdaInternship/Assets/Meshes/TileMesh.TileMesh'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>TileMaterialAssetRef(TEXT("Material'/Game/BethesdaInternship/Assets/Materials/M_Tile.M_Tile'"));

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	RootComponent = SceneRoot;
	
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	UStaticMesh* TileMeshAsset = TileMeshAssetRef.Object;
	TileMaterialAsset = TileMaterialAssetRef.Object;
	TileMesh->SetStaticMesh(TileMeshAsset);\
	TileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	HoverTimerDel.BindUFunction(this, FName("OnHoverTimerEnd"));

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	TileMaterialInstance = TileMesh->CreateDynamicMaterialInstance(0, TileMaterialAsset);
	TileMesh->SetMaterial(0, TileMaterialInstance);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHovered)
	{
		OnHoverStart();
	}
	else if (bHoverFade)
	{
		float CurrentHoverOpacity;
		TileMaterialInstance->GetScalarParameterValue(FName("Hovered"), CurrentHoverOpacity);;
		TileMaterialInstance->SetScalarParameterValue(FName("Hovered"), FMath::Lerp(CurrentHoverOpacity, 0.f, 0.4f));
		if (CurrentHoverOpacity <= 0.0f)
		{
			bHoverFade = false;
		}
	}
	else
	{
		// Do Nothing
	}

}

void ATile::OnHoverStart()
{
	TileMaterialInstance->SetScalarParameterValue(FName("Hovered"), 10.f);
}

void ATile::OnHoverEnd()
{
}

void ATile::StartHovering()
{
	bIsHovered = true;
	GetWorldTimerManager().SetTimer(HoverTimerHandle, HoverTimerDel, 0.1f, false);
}

void ATile::EndHovering()
{
	
}

void ATile::SetTileColor(FLinearColor Color)
{
	TileMaterialInstance->SetVectorParameterValue(FName("TileColor"), Color);
}

FIntPoint ATile::GetCoordinates()
{
	return FIntPoint(PositionX, PositionY);
}

void ATile::OnHoverTimerEnd()
{
	bIsHovered = false;
	bHoverFade = true;
}

