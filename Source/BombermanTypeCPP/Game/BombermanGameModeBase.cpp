// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanGameModeBase.h"
#include "BombermanPlayer.h"
#include "BombermanTypeCPP/Map/MapGenerator.h"
#include "BombermanTypeCPP/Map/MapGrid.h"

void ABombermanGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	AActor* MapGenActor = GetWorld()->SpawnActor<AActor>(MapGenerator, FVector(), FRotator());

	AMapGenerator* MapGen = Cast<AMapGenerator>(MapGenActor);

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