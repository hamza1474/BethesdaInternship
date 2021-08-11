// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Tile.h"
#include "DoorTile.h"
#include "FloorTile.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Reverse.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABethesdaInternshipGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!ensure(GameMode != nullptr))
		return;

}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<ATile*> AGrid::GetTileGridArray_Implementation()
{
	return TileGridArray;
}

void AGrid::GenerateGrid()
{
	if (!ensure(GameMode!=nullptr))
		return;

	for (ATile* Tile : TileGridArray)
	{
		Tile->Destroy();
	}
	TileGridArray.Empty();
	
	if (!ensure(GetWorld() != nullptr))
	{
		return;
	}
	
	for (int i = 0; i < GridSizeVertical; i++)
	{
		for (int j = 0; j < GridSizeHorizontal; j++)
		{
			if(i == 4 && j == 6)
			{
				ATile* Tile = GetWorld()->SpawnActor<ADoorTile>(DoorTileBlueprint, FVector(((TileSize + GridOffset) * j),((TileSize + GridOffset) * i), 0), FRotator(0));
				Tile->PositionX = j;
				Tile->PositionY = i;
				TileGridArray.Add(Tile);
			}
			else
			{
				ATile* Tile = GetWorld()->SpawnActor<AFloorTile>(FloorTileBlueprint, FVector(((TileSize + GridOffset) * j),((TileSize + GridOffset) * i), 0), FRotator(0));
				Tile->PositionX = j;
				Tile->PositionY = i;
				TileGridArray.Add(Tile);
			}
		}
	}
}

bool AGrid::FindPath(ATile* StartTile, ATile* EndTile)
{
	FIntPoint CurrentPosition = StartTile->GetCoordinates();
	FIntPoint GoalPosition = EndTile->GetCoordinates();

	// Directions To Traverse
	TArray<FIntPoint> Directions({
        FIntPoint(0,1),
        FIntPoint(0,-1),
        FIntPoint(1,0),
        FIntPoint(-1,0)
		
    });


	// Array to Store Visited Nodes
	//TQueue<FIntPoint> Visited;
	
	// Array to Store Queue
	TQueue<FIntPoint> Queue;
	TQueue<FIntPoint> Neighbours;
	
	Queue.Enqueue(CurrentPosition);
	GetTileAtCoordinate(CurrentPosition)->bVisited = true;
	
	// Traverse

	while (!Queue.IsEmpty())
	{
		if (EndTile->bVisited)
		{
			UE_LOG(LogTemp, Warning, TEXT("ENDTILEEEEEEEEEEEEEE"))	
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Starting Iteration"), Queue.IsEmpty())
		FIntPoint CurrentNode;
		Queue.Dequeue(CurrentNode);
		UE_LOG(LogTemp, Warning, TEXT("Current Node is %i, %i "), CurrentNode.X, CurrentNode.Y)
		UE_LOG(LogTemp, Warning, TEXT("Queeu Empty %i"), Queue.IsEmpty())
		for (FIntPoint Direction : Directions)
		{
			FIntPoint Result = CurrentNode + Direction;
				
			if (IsValidCoordinate(Result) && !GetTileAtCoordinate(Result)->bVisited)
			{
				GetTileAtCoordinate(Result)->Parent = GetTileAtCoordinate(CurrentNode);
				GetTileAtCoordinate(Result)->bVisited = true;
				UE_LOG(LogTemp, Warning, TEXT("Found Neighbour %i, %i"), Result.X, Result.Y)
				Queue.Enqueue(Result);
			}
		}
	}


		/*	
		for (int i = Queue.Num() - 1; i >= 0; i--)
		{
			if (Visited.Contains(GoalPosition))
			{
				return true;
			}
			
			FIntPoint CurrentNode = Queue[i];
			for (FIntPoint Direction : Directions)
			{
				FIntPoint Result = CurrentNode + Direction;
				
				if (IsValidCoordinate(Result) && !Visited.Contains(Result))
				{
					GetTileAtCoordinate(Result)->Parent = GetTileAtCoordinate(CurrentNode);
					Queue.Push(Result);
				}
			}
			Visited.Push(CurrentNode);
		}
		for (FIntPoint Point : Visited)
		{
			Queue.Remove(Point);
		}
	}
	// TODO: Need another check for Visited[-1] here?
	*/
	return false;
}

void AGrid::ColorPath(ATile* EndTile)
{
	ATile* Temp = EndTile;	
	while (Temp->Parent != nullptr)
	{
		
		Temp->SetTileColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
		Temp = Temp->Parent;
	}
		
}

void AGrid::SetChild(ATile* EndTile)
{
	ATile* Temp = EndTile;
	while (Temp->Parent != nullptr)
	{
		Temp->Parent->Child = Temp;
		Temp = Temp->Parent;
	}
}


FIntPoint AGrid::IndexToCoordinate(int Index)
{
	FIntPoint Result;
	Result.Y = Index / GridSizeHorizontal;
	Result.X = Index % GridSizeHorizontal;
	return Result;
}

int AGrid::CoordinateToIndex(FIntPoint Coord)
{
	return (Coord.Y * GridSizeHorizontal) + Coord.X;
}

bool AGrid::IsValidCoordinate(FIntPoint Coord)
{
	if (Coord.X >= 0 && Coord.X < GridSizeHorizontal && Coord.Y >= 0 && Coord.Y < GridSizeVertical)
	{
		return true;
	}
	return false;
}

ATile* AGrid::GetTileAtCoordinate(FIntPoint Coord)
{
	return TileGridArray[CoordinateToIndex(Coord)];
}

void AGrid::SetTileAreaColor(ATile* Center, int Area)
{
}

