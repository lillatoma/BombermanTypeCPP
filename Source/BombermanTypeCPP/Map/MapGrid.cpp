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


UMapGridpoint* AMapGrid::GetPointOnGrid(int32 x, int32 y)
{
	//To understand this, look at indexes visualized in GenerateMapGrid
	return Gridpoints[y * Size.X + x];
}

UMapGridpoint* AMapGrid::GetPointOnGrid(FIntPoint coord)
{
	return Gridpoints[coord.Y * Size.X + coord.X];
}

TArray<class UMapGridpoint*>* AMapGrid::GenerateMapGrid()
{
	//////////////////////
	// Indexes visualized
	//////////////////////
	// 12 13 14 15
	// 8  9  10 11
	// 4  5  6  7
	// 0  1  2  3
	/////////////////////


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

FVector AMapGrid::ConvertGridToWorld(FIntPoint coord) const
{
	return FVector(GridDistance * coord.X, GridDistance * coord.Y, 0.f);
}

FVector AMapGrid::GetClosestGridCenter(FVector Location) const
{
	return ConvertGridToWorld(FIntPoint((Location.X + GridDistance/2) / GridDistance, (Location.Y + GridDistance/2) / GridDistance));
}

FIntPoint AMapGrid::GetClosestGridPoint(FVector Location) const
{
	//Adding (0.5f* GridDistance,0.5f*GridDistance,0) to fix the calculation
	return FIntPoint((Location.X + GridDistance/2) / GridDistance, (Location.Y + GridDistance/2) / GridDistance);
}

FVector AMapGrid::GetGridMiddle() const
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

bool AMapGrid::IsPointOnGrid(FIntPoint point) const
{
	//Returns true if the point has coordinates that are on the grid
	return (point.X >= 0 && point.Y >= 0 && point.X < Size.X && point.Y < Size.Y);

}

TArray<FIntPoint> AMapGrid::GetPointNeighbors(FIntPoint coord, bool includePlayers)
{

	if (!includePlayers)
	{
		//If include players is false
		//Then it checks if the neighboring gridpoint is air, has no bomb placed there, and there is no player there
		//If these conditions are met, then its a true neighbor
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
	{	//If include players is false
		//Then it checks if the neighboring gridpoint is air, and has no bomb placed there
		//If these conditions are met, then its a true neighbor
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
	//If there is no object to break, every place on the map is reachable (except solids)
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
	//If there is a breakable left, then we should be looking to find from an Air Gridpoint
	else if (GetPointOnGrid(coord)->Type == EMGPMapGridpointType::Air)
	{
		//First we initialize our gridpoints found
		//The original point is always reachable, since its Air, so add it
		TArray<FIntPoint> GridpointsFound;
		GridpointsFound.Reserve((Size.X - 2) * (Size.Y - 2));
		GridpointsFound.Add(coord);

		//Then we check if we have neighbors
		//If we have, that means we can continue the search on those points
		TArray<FIntPoint> UncheckedPoints = GetPointNeighbors(coord, includePlayers);
		UncheckedPoints.Reserve((Size.X - 2) * (Size.Y - 2));
				
		for (; UncheckedPoints.Num() > 0;)
		{
			FIntPoint last = UncheckedPoints.Last(0);
			
			//If this point is not added yet, we add it
			//Then remove it from the unchecked points
			//And run the search on its neighbors
			//If the neighbors are not added to the reachable gridpoints
			//Then we add it to check those points later
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
				//Same as in GetReachablePoints but with safety check
				if (GetPointOnGrid(x, y)->Type == EMGPMapGridpointType::Air && IsSafePoint(FIntPoint(x,y)))
					Pts.Add(FIntPoint(x, y));

		return Pts;
	}
	//Same as in GetReachablePoints, but with safety check at the end
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

		//If a point is safe, then we add it to safepoints
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
	//This function checks all 4 directions if a bomb's explosion could reach there
	//Assuming the bomb's explosion length is infinite
	//TODO: calculate with breakables:
	//Example for todo: there is one breakable, but 2 bombs on the other side, then it's not safe
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
	//This function gives a score for a point on the grid according to how many breakables it can break
	//TODO: Chain calculation for if a bomb could be in expRange
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
	//This function rates every point in the points array
	//Returns one randomly from the best ones

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

	//Archive: Not deleting this part below, 
	//         because if chain bomb calculations are added, 
	//         this is better than the above code

	
	/*
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
	*/

}


TArray<FIntPoint> AMapGrid::GetReachablePowerupList(TArray<FIntPoint>& points)
{
	//Assuming that points is the array of reachable points
	TArray<FIntPoint> Powerups;

	//If no powerups are available, then we can't find one
	if (PowerupsAvailable.Num() == 0)
		return Powerups;
	
	//Checking every point of points if they have a powerup
	for (FIntPoint point : points)
	{
		if (PowerupsAvailable.Find(point) != INDEX_NONE)
			Powerups.Add(point);
	}
	return Powerups;

}

TArray<FIntPoint> AMapGrid::GetReachablePlayerList(TArray<FIntPoint>& points, int excludePlayer)
{
	//Assuming that points is the array of reachable points, and excludePlayer is the playerID that we don't calculate with
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
	//This function returns an array of points that can be seen from a point in expRange distance
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
	//Returns the intersection of two arrays
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

TArray<FVector> AMapGrid::GetFourCorners() const
{
	//Returns the four corners of the map
	TArray<FVector> Corners;
	Corners.Add(ConvertGridToWorld(FIntPoint(1, 1)));
	Corners.Add(ConvertGridToWorld(FIntPoint(Size.X - 2, 1)));
	Corners.Add(ConvertGridToWorld(FIntPoint(Size.X - 2, Size.Y - 2)));
	Corners.Add(ConvertGridToWorld(FIntPoint(1, Size.Y - 2)));

	return Corners;
}
