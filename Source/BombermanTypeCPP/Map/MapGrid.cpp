// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGrid.h"
#include "MapGridpoint.h"
#include "Kismet/KismetArrayLibrary.h"


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

void AMapGrid::AddBreakable()
{
	BreakableCount++;
}

void AMapGrid::RemoveBreakable()
{
	BreakableCount--;
}

bool AMapGrid::IsPointOnGrid(FIntPoint point)
{
	return (point.X >= 0 && point.Y >= 0 && point.X < Size.X && point.Y < Size.Y);


}

TArray<FIntPoint> AMapGrid::GetPointNeighbors(FIntPoint coord, bool includePlayers)
{

	if (!includePlayers)
	{

		TArray<FIntPoint> Neighbors;

		if (IsPointOnGrid(FIntPoint(coord.X, coord.Y - 1)) &&
			GetPointOnGrid(FIntPoint(coord.X, coord.Y - 1))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X, coord.Y - 1)) == INDEX_NONE
			&& Players.Find(FIntPoint(coord.X, coord.Y - 1)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X, coord.Y - 1));

		if (IsPointOnGrid(FIntPoint(coord.X, coord.Y + 1)) &&
			GetPointOnGrid(FIntPoint(coord.X, coord.Y + 1))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X, coord.Y + 1)) == INDEX_NONE
			&& Players.Find(FIntPoint(coord.X, coord.Y + 1)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X, coord.Y + 1));

		if (IsPointOnGrid(FIntPoint(coord.X - 1, coord.Y)) &&
			GetPointOnGrid(FIntPoint(coord.X - 1, coord.Y))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X - 1, coord.Y)) == INDEX_NONE
			&& Players.Find(FIntPoint(coord.X - 1, coord.Y)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X - 1, coord.Y));

		if (IsPointOnGrid(FIntPoint(coord.X + 1, coord.Y)) &&
			GetPointOnGrid(FIntPoint(coord.X + 1, coord.Y))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X + 1, coord.Y)) == INDEX_NONE
			&& Players.Find(FIntPoint(coord.X + 1, coord.Y)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X + 1, coord.Y));


		return Neighbors;
	}
	else
	{
		TArray<FIntPoint> Neighbors;

		if (IsPointOnGrid(FIntPoint(coord.X, coord.Y - 1)) &&
			GetPointOnGrid(FIntPoint(coord.X, coord.Y - 1))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X, coord.Y - 1)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X, coord.Y - 1));

		if (IsPointOnGrid(FIntPoint(coord.X, coord.Y + 1)) &&
			GetPointOnGrid(FIntPoint(coord.X, coord.Y + 1))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X, coord.Y + 1)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X, coord.Y + 1));

		if (IsPointOnGrid(FIntPoint(coord.X - 1, coord.Y)) &&
			GetPointOnGrid(FIntPoint(coord.X - 1, coord.Y))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X - 1, coord.Y)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X - 1, coord.Y));

		if (IsPointOnGrid(FIntPoint(coord.X + 1, coord.Y)) &&
			GetPointOnGrid(FIntPoint(coord.X + 1, coord.Y))->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(FIntPoint(coord.X + 1, coord.Y)) == INDEX_NONE)
			Neighbors.Add(FIntPoint(coord.X + 1, coord.Y));

		return Neighbors;
	}
}

TArray<FIntPoint> AMapGrid::GetReachablePoints(FIntPoint coord, bool includePlayers)
{
	if (BreakableCount == 0)
	{
		TArray<FIntPoint> Pts;
		Pts.Reserve((Size.X - 2) * (Size.Y - 2));

		for (int x = 1; x < Size.X - 1; x++)
			for (int y = 1; y < Size.Y - 1; y++)
				if (GetPointOnGrid(x, y)->Type == EMGPMapGridpointType::Air)
					Pts.Add(FIntPoint(x, y));

		return Pts;
	}

	else if (GetPointOnGrid(coord)->Type == EMGPMapGridpointType::Air)
	{
		TArray<FIntPoint> GridpointsFound;
		GridpointsFound.Reserve((Size.X - 2) * (Size.Y - 2));
		GridpointsFound.Add(coord);


		TArray<FIntPoint> UncheckedPoints = GetPointNeighbors(coord, includePlayers);
		UncheckedPoints.Reserve((Size.X - 2) * (Size.Y - 2));
				
		for (; UncheckedPoints.Num() > 0;)
		{
			FIntPoint last = UncheckedPoints.Last(0);
			
			
			if (GridpointsFound.Find(last) == INDEX_NONE)
			{
				GridpointsFound.Add(last);
				UncheckedPoints.Remove(last);
				TArray<FIntPoint> neighbors = GetPointNeighbors(last, includePlayers);
				
				for (int i = 0; i < neighbors.Num(); i++)
					if (GridpointsFound.Find(neighbors[i]) == INDEX_NONE)
						UncheckedPoints.Add(neighbors[i]);
			}
			else UncheckedPoints.Remove(last);
			
		}



		return GridpointsFound;
	}
	return TArray<FIntPoint>();
}

TArray<FIntPoint> AMapGrid::GetSafeReachablePoints(FIntPoint coord, bool includePlayers)
{
	if (BreakableCount == 0)
	{
		TArray<FIntPoint> Pts;
		Pts.Reserve((Size.X - 2) * (Size.Y - 2));

		for (int x = 1; x < Size.X - 1; x++)
			for (int y = 1; y < Size.Y - 1; y++)
				if (GetPointOnGrid(x, y)->Type == EMGPMapGridpointType::Air && IsSafePoint(FIntPoint(x,y)))
					Pts.Add(FIntPoint(x, y));

		return Pts;
	}

	else if (GetPointOnGrid(coord)->Type == EMGPMapGridpointType::Air)
	{
		TArray<FIntPoint> GridpointsFound;
		GridpointsFound.Reserve((Size.X - 2) * (Size.Y - 2));
		GridpointsFound.Add(coord);


		TArray<FIntPoint> UncheckedPoints = GetPointNeighbors(coord, includePlayers);
		UncheckedPoints.Reserve((Size.X - 2) * (Size.Y - 2));

		for (; UncheckedPoints.Num() > 0;)
		{
			FIntPoint last = UncheckedPoints.Last(0);


			if (GridpointsFound.Find(last) == INDEX_NONE)
			{
				GridpointsFound.Add(last);
				UncheckedPoints.Remove(last);
				TArray<FIntPoint> neighbors = GetPointNeighbors(last, includePlayers);

				for (int i = 0; i < neighbors.Num(); i++)
					if (GridpointsFound.Find(neighbors[i]) == INDEX_NONE)
						UncheckedPoints.Add(neighbors[i]);
			}
			else UncheckedPoints.Remove(last);

		}

		TArray<FIntPoint> SafePoints;
		int Total = GridpointsFound.Num();
		SafePoints.Reserve(Total);
		for (int i = 0; i < Total; i++)
		{
			if (IsSafePoint(GridpointsFound[i]))
				SafePoints.Add(GridpointsFound[i]);

		}


		return SafePoints;
	}
	return TArray<FIntPoint>();
}

void AMapGrid::AddBomb(FIntPoint point)
{
	BombsPlaced.Add(point);
}

void AMapGrid::DeleteBomb(FIntPoint point)
{
	BombsPlaced.Remove(point);
}

bool AMapGrid::IsSafePoint(FIntPoint point)
{
	for (int b = 0; b < BombsPlaced.Num(); b++)
	{
		if (point == BombsPlaced[b])
			return false;
		for (int x = BombsPlaced[b].X - 1; x > 0; x--)
		{
			FIntPoint expPoint = FIntPoint(x, BombsPlaced[b].Y);
			if (GetPointOnGrid(expPoint)->Type != EMGPMapGridpointType::Air)
				break;
			if (expPoint == point)
				return false;
		}
		for (int x = BombsPlaced[b].X + 1; x < Size.X - 1; x++)
		{
			FIntPoint expPoint = FIntPoint(x, BombsPlaced[b].Y);
			if (GetPointOnGrid(expPoint)->Type != EMGPMapGridpointType::Air)
				break;
			if (expPoint == point)
				return false;
		}
		for (int y = BombsPlaced[b].Y - 1; y > 0; y--)
		{
			FIntPoint expPoint = FIntPoint(BombsPlaced[b].X,y);
			if (GetPointOnGrid(expPoint)->Type != EMGPMapGridpointType::Air)
				break;
			if (expPoint == point)
				return false;
		}
		for (int y = BombsPlaced[b].Y + 1; y < Size.Y - 1; y++)
		{
			FIntPoint expPoint = FIntPoint(BombsPlaced[b].X, y);
			if (GetPointOnGrid(expPoint)->Type != EMGPMapGridpointType::Air)
				break;
			if (expPoint == point)
				return false;
		}
	}


	return true;
}

bool AMapGrid::IsBombOnPoint(FIntPoint point)
{
	return BombsPlaced.Find(point) != INDEX_NONE;
}


int AMapGrid::RatePointForExplosion(FIntPoint point, int expRange)
{
	if (IsBombOnPoint(point))
		return 0;

	int BreakablesFound = 0;

	int expLeft = expRange - 1;
	for (int x = point.X - 1; x > 0 && expLeft > 0; x--)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(x, point.Y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Breakable)
		{
			BreakablesFound++;
			break;
		}
		else if (IsBombOnPoint(pt))
			BreakablesFound++;
	}

	expLeft = expRange - 1;
	for (int x = point.X + 1; x < Size.X - 1 && expLeft > 0; x++)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(x, point.Y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Breakable)
		{
			BreakablesFound++;
			break;
		}
		else if (IsBombOnPoint(pt))
			BreakablesFound++;
	}
	expLeft = expRange - 1;
	for (int y = point.Y - 1; y > 0 && expLeft > 0; y--)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(point.X, y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Breakable)
		{
			BreakablesFound++;
			break;
		}
		else if (IsBombOnPoint(pt))
			BreakablesFound++;
	}
	expLeft = expRange - 1;
	for (int y = point.Y + 1; y < Size.Y - 1 && expLeft > 0; y++)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(point.X,y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Breakable)
		{
			BreakablesFound++;
			break;
		}
		else if (IsBombOnPoint(pt))
			BreakablesFound++;
	}

	return BreakablesFound;
}

FIntPoint AMapGrid::GetBestPointForExplosion(TArray<FIntPoint>& points, int expRange)
{

	TArray<FIntPoint> Rated3;
	TArray<FIntPoint> Rated2;
	TArray<FIntPoint> Rated1;
	TArray<FIntPoint> Rated0;
	int TNum = points.Num();
	for (int i = 0; i < TNum; i++)
	{
		int rating = RatePointForExplosion(points[i],expRange);
		if (rating >= 3)
			Rated3.Add(points[i]);
		else if (rating == 2)
			Rated2.Add(points[i]);
		else if (rating == 1)
			Rated1.Add(points[i]);
		else if (rating == 0)
			Rated0.Add(points[i]);
	}

	if (Rated3.Num() > 0)
		return Rated3[FMath::RandRange(0, Rated3.Num() - 1)];
	else if (Rated2.Num() > 0)
		return Rated2[FMath::RandRange(0, Rated2.Num() - 1)];
	else if (Rated1.Num() > 0)
		return Rated1[FMath::RandRange(0, Rated1.Num() - 1)];
	else 
		return Rated0[FMath::RandRange(0, Rated0.Num() - 1)];
	//TArray<int> R;
	//R.Reserve(points.Num());
	//for (int i = 0; i < points.Num(); i++)
	//	R.Add(i);

	//int LastIndex = R.Num() - 1;
	//for (int i = 0; i <= LastIndex; i++)
	//{
	//	int32 Index = FMath::RandRange(i, LastIndex);
	//	if (Index != i)
	//	{
	//		R.Swap(i, Index);
	//	}
	//}

	////UKismetArrayLibrary::Array_Shuffle(R);

	//int bestIndex = 0;
	//int bombScore = RatePointForExplosion(points[R[0]], expRange);

	//for (int i = 1; i <= LastIndex; i++)
	//{
	//	int score = RatePointForExplosion(points[R[i]], expRange);
	//	if (score > bombScore)
	//	{
	//		bombScore = score;
	//		bestIndex = i;
	//	}
	//}

	//return points[R[bestIndex]];

	int32 LastIndex = points.Num() - 1;

	for (int32 i = 0; i <= LastIndex; i++)
	{
		int32 index = FMath::RandRange(i, LastIndex);
		if (i != index)
			points.Swap(i, index);
	}

	int bestIndex = 0;
	int bombScore = RatePointForExplosion(points[0], expRange);

	for (int i = 1; i <= LastIndex; i++)
	{
		int score = RatePointForExplosion(points[i], expRange);
		if (score > bombScore)
		{
			bombScore = score;
			bestIndex = i;
		}
	}

	return points[bestIndex];

}


TArray<FIntPoint> AMapGrid::GetReachablePowerupList(TArray<FIntPoint>& points)
{
	TArray<FIntPoint> Powerups;

	if (PowerupsAvailable.Num() == 0)
		return Powerups;
	
	for (FIntPoint point : points)
	{
		if (PowerupsAvailable.Find(point) != INDEX_NONE)
			Powerups.Add(point);
	}
	return Powerups;

}

TArray<FIntPoint> AMapGrid::GetReachablePlayerList(TArray<FIntPoint>& points, int excludePlayer)
{
	TArray<FIntPoint> ReachablePlayers;

	int Num = points.Num();
	for (int i = 0; i < Num; i++)
	{
		int Index = Players.Find(points[i]);
		if (Index != INDEX_NONE && Index != excludePlayer)
			ReachablePlayers.Add(points[i]);
	}

	return ReachablePlayers;
}

TArray<FIntPoint> AMapGrid::GetPointsOverseeing(FIntPoint point, int expRange)
{
	TArray<FIntPoint> Points;

	int expLeft = expRange - 1;
	for (int x = point.X - 1; x > 0 && expLeft > 0; x--)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(x, point.Y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(pt) == INDEX_NONE)
		{
			Points.Add(pt);
		}
		else break;
	}
	expLeft = expRange - 1;
	for (int x = point.X + 1; x < Size.X - 1 && expLeft > 0; x++)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(x, point.Y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(pt) == INDEX_NONE)
		{
			Points.Add(pt);
		}
		else break;
	}
	expLeft = expRange - 1;
	for (int y = point.Y - 1; y > 0 && expLeft > 0; y--)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(point.X, y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(pt) == INDEX_NONE)
		{
			Points.Add(pt);
		}
		else break;
	}
	expLeft = expRange - 1;
	for (int y = point.Y + 1; y < Size.Y && expLeft > 0; y++)
	{
		expLeft--;
		FIntPoint pt = FIntPoint(point.X, y);
		if (GetPointOnGrid(pt)->Type == EMGPMapGridpointType::Air
			&& BombsPlaced.Find(pt) == INDEX_NONE)
		{
			Points.Add(pt);
		}
		else break;
	}

	return Points;
}

TArray<FIntPoint> AMapGrid::Intersection(TArray<FIntPoint> A, TArray<FIntPoint> B)
{
	TArray<FIntPoint> I;

	int NumA = A.Num(), NumB = B.Num();

	for(int a = 0; a < NumA; a++)
		for (int b = 0; b < NumB; b++)
		{
			if (A[a] == B[b])
				I.Add(A[a]);
		}
	return I;
}

void AMapGrid::AddPowerup(FIntPoint point)
{
	PowerupsAvailable.Add(point);
}

void AMapGrid::DeletePowerup(FIntPoint point)
{
	PowerupsAvailable.Remove(point);
}

int AMapGrid::GetPowerupCount()
{
	return PowerupsAvailable.Num();
}

int AMapGrid::GetPlayerIndex()
{
	NextPlayerIndex++;
	AddCharacter();
	return NextPlayerIndex - 1;
}

void AMapGrid::AddCharacter()
{
	Players.Add(FIntPoint());
}

void AMapGrid::UpdatePlayerPosition(int character, FIntPoint point)
{
	Players[character] = point;
}

int AMapGrid::GetBreakableCount() const
{
	return BreakableCount;
}
