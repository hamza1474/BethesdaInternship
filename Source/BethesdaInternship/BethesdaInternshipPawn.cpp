// Fill out your copyright notice in the Description page of Project Settings.


#include "BethesdaInternshipPawn.h"
#include "Components/DecalComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Tile.h"

// Sets default values
ABethesdaInternshipPawn::ABethesdaInternshipPawn()
{
}

// Called when the game starts or when spawned
void ABethesdaInternshipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABethesdaInternshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void ABethesdaInternshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

