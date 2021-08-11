// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorTile.h"

ADoorTile::ADoorTile()
{
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DoorMeshAssetRef(TEXT("StaticMesh'/Game/BethesdaInternship/Assets/Meshes/Door.Door'"));

	UStaticMesh* TileMeshAsset = DoorMeshAssetRef.Object;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Door"));
	DoorMesh->SetStaticMesh(TileMeshAsset);
	DoorMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DoorMesh->AddRelativeLocation(FVector(-50.f ,0.f, 0.f));
	TileType = ETileType::Door;
	
}
