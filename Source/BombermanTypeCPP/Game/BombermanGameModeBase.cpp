// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanGameModeBase.h"
#include "BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGenerator.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

void ABombermanGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//Try spawning the mapgenerator
	AActor* MapGenActor = GetWorld()->SpawnActor<AActor>(MapGenerator, FVector(), FRotator());

	AMapGenerator* MapGen = Cast<AMapGenerator>(MapGenActor);

	//If it is successful
	//We get the four corners of the grid
	//Then spawn the player and as many AIs as needed in random corners
	if (MapGenActor)
	{
		AMapGrid* Grid = MapGen->GetGrid();

		if (Grid)
		{
			TArray<FVector> SpawnPoints = Grid->GetFourCorners();

			if (Player)
			{
				int r = FMath::RandRange(0, SpawnPoints.Num() - 1);
				AActor* PlayerActor = GetWorld()->SpawnActor<AActor>(Player, SpawnPoints[r], FRotator());
				SpawnPoints.RemoveAt(r);
			}
			if (EnemyPlayer)
			{
				for (int i = 0; i < EnemyPlayerCount; i++)
				{
					int r = FMath::RandRange(0, SpawnPoints.Num() - 1);
					AActor* EnemyActor = GetWorld()->SpawnActor<AActor>(EnemyPlayer, SpawnPoints[r], FRotator());
					SpawnPoints.RemoveAt(r);
				}
			}
		}
	}


}