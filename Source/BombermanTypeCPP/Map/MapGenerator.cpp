// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "MapGrid.h"
#include "MapGridpoint.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	AActor* Grid = GetWorld()->SpawnActor(AMapGrid::StaticClass());
	MapGrid = Cast<AMapGrid>(Grid);

	GenerateMap();
}

void AMapGenerator::GenerateMap()
{
	MapGrid->SetSize(Size);
	MapGrid->SetGridDistance(GridDistance);
	MapGrid->GenerateMapGrid();
	
	//////////////////
	// Generate Floor
	// It is a single, large-scaled solid block
	//////////////////

	{
		FVector Position = MapGrid->GetGridMiddle();
		Position.Z -= MapGrid->GetGridDistance();
		AActor* Object = GetWorld()->SpawnActor<AActor>(SolidObject, Position, FRotator());
		Object->SetActorScale3D(FVector(Size.X, Size.Y, 1));
	}
	//////////////////
	//Generate Outline
	//////////////////

	//Horizontal lines
	for (int x = 0; x < Size.X; x++)
	{
		//"Bottom", filled up with solid blocks
		MapGrid->GetPointOnGrid(x, 0)->Type = EMGPMapGridpointType::Solid;
		FVector Position = MapGrid->ConvertGridToWorld(FIntPoint(x, 0));
		AActor* Object = GetWorld()->SpawnActor<AActor>(SolidObject, Position, FRotator());
		MapGrid->GetPointOnGrid(x, 0)->Block = Object;

		//"Top", filled up with solid blocks
		MapGrid->GetPointOnGrid(x, Size.Y - 1)->Type = EMGPMapGridpointType::Solid;
		FVector Position2 = MapGrid->ConvertGridToWorld(FIntPoint(x, Size.Y - 1));
		AActor* Object2 = GetWorld()->SpawnActor<AActor>(SolidObject, Position2, FRotator());
		MapGrid->GetPointOnGrid(x, Size.Y - 1)->Block = Object2;
	}

	//Vertical lines
	for (int y = 0; y < Size.Y; y++)
	{
		MapGrid->GetPointOnGrid(0, y)->Type = EMGPMapGridpointType::Solid;
		FVector Position = MapGrid->ConvertGridToWorld(FIntPoint(0, y));
		AActor* Object = GetWorld()->SpawnActor<AActor>(SolidObject, Position, FRotator());
		MapGrid->GetPointOnGrid(0, y)->Block = Object;

		MapGrid->GetPointOnGrid(Size.X - 1, y)->Type = EMGPMapGridpointType::Solid;
		FVector Position2 = MapGrid->ConvertGridToWorld(FIntPoint(Size.X - 1, y));
		AActor* Object2 = GetWorld()->SpawnActor<AActor>(SolidObject, Position2, FRotator());
		MapGrid->GetPointOnGrid(Size.X - 1, y)->Block = Object2;
	}

	//Dot pattern
	
	for (int x = 2; x < Size.X - 1; x+=2)
		for (int y = 2; y < Size.Y - 1; y+= 2)
		{
			MapGrid->GetPointOnGrid(x, y)->Type = EMGPMapGridpointType::Solid;
			FVector Position = MapGrid->ConvertGridToWorld(FIntPoint(x, y));
			AActor* Object = GetWorld()->SpawnActor<AActor>(SolidObject, Position, FRotator());
			MapGrid->GetPointOnGrid(x, y)->Block = Object;
		}

	//Random Breakables

	FVector2D Midpoint = FVector2D(0.5f * Size.X, 0.5f * Size.Y);


	for (int x = 1; x < Size.X - 1; x ++)
		for (int y = 1; y < Size.Y - 1; y++)
		{
			//Corner Check
			//If its in a corner 2x2, continue
			if ((x <= 2 && y <= 2)
				|| (x <= 2 && y >= Size.Y - 3)
				|| (x >= Size.X - 3 && y <= 2)
				|| (x >= Size.X - 3 && y >= Size.Y - 3))
				continue;

			//If its on a dot solid, continue
			//Dot Check
			if (x % 2 == 0 && y % 2 == 0)
				continue;

			double distSquared = FVector2D::DistSquared(Midpoint, FVector2D(x, y));

			//By default, it starts with outer circle's chance
			float chance = outerCircleBreakableChance;

			//If the distance is smaller then the inner circle, spawnchance is innercircle's spawn chance
			if (distSquared < innerCircleRadius * innerCircleRadius)
				chance = innerCircleBreakableChance;

			//If the distance is between the inner and outer circle, linear interpolate the chance with the radiuses
			else if (distSquared < outerCircleRadius * outerCircleRadius)
			{
				double dist = FMath::Sqrt(distSquared);
				float p = (dist - innerCircleRadius) / (outerCircleRadius - innerCircleRadius);
				chance = p * outerCircleBreakableChance + (1.f - p) * innerCircleBreakableChance;
			}

			//After we finally got the spawn chance, generate a random number to see if it should generate
			float r = FMath::RandRange(0.f, 1.f);
			if (r < chance)
			{
				//And if it should, spawn a new breakable
				MapGrid->GetPointOnGrid(x, y)->Type = EMGPMapGridpointType::Breakable;
				FVector Position = MapGrid->ConvertGridToWorld(FIntPoint(x, y));
				AActor* Object = GetWorld()->SpawnActor<AActor>(BreakableObject, Position, FRotator());
				MapGrid->GetPointOnGrid(x, y)->Block = Object;
				MapGrid->AddBreakable();
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("(%d|%d) | RR: %f, Chance: %f"), x, y, r, chance));
			}
		}



}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AMapGrid* AMapGenerator::GetGrid() const
{
	return MapGrid;
}

