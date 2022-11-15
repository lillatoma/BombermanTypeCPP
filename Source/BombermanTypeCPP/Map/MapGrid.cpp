// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"
#include "MapGridpoint.h"

// Sets default values
AMapGrid::AMapGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMapGrid::BeginPlay()
{
	Super::BeginPlay();
	
}


UMapGridpoint* AMapGrid::GetPointOnGrid(int x, int y)
{
	return Gridpoints[y * Size.X + x];
}

UMapGridpoint* AMapGrid::GetPointOnGrid(FIntPoint coord)
{
	return Gridpoints[coord.Y * Size.X + coord.X];
}

TArray<class UMapGridpoint*>* AMapGrid::GenerateMapGrid()
{
	for (int y = 0; y < Size.Y; y++)
	{
		for (int x = 0; x < Size.X; x++)
		{
			UMapGridpoint* point = NewObject<UMapGridpoint>(UMapGridpoint::StaticClass());
			point->Coord = FIntPoint(x, y);
			Gridpoints.Add(point);
		}
	}

	return &Gridpoints;
}

void AMapGrid::SetSize(int32 x, int32 y)
{
	Size = FIntPoint(x, y);
}

void AMapGrid::SetSize(FIntPoint size)
{
	this->Size = size;
}

FIntPoint AMapGrid::GetSize() const
{
	return Size;
}

float AMapGrid::GetGridDistance() const
{
	return GridDistance;
}

void AMapGrid::SetGridDistance(float gridDistance)
{
	this->GridDistance = gridDistance;
}

FVector AMapGrid::ConvertGridToWorld(FIntPoint coord)
{
	return FVector(coord.X * GridDistance, coord.Y * GridDistance, 0.f);
}

FVector AMapGrid::GetClosestGridCenter(FVector Location)
{
	return ConvertGridToWorld(FIntPoint((Location.X + GridDistance/2) / GridDistance, (Location.Y + GridDistance/2) / GridDistance));
}

FIntPoint AMapGrid::GetClosestGridPoint(FVector Location)
{
	return FIntPoint((Location.X + GridDistance/2) / GridDistance, (Location.Y + GridDistance/2) / GridDistance);
}

FVector AMapGrid::GetGridMiddle()
{
	return FVector(Size.X * GridDistance / 2, Size.Y * GridDistance / 2,0);
}

