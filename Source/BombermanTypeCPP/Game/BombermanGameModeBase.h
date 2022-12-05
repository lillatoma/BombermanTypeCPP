// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BombermanGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANTYPECPP_API ABombermanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Map Builder")
		TSubclassOf<class AMapGenerator> MapGenerator;
	
	UPROPERTY(EditAnywhere, Category = "Player Characters")
		TSubclassOf<class ABombermanPlayer> Player;
	UPROPERTY(EditAnywhere, Category = "Player Characters")
		TSubclassOf<class ABombermanPlayer> EnemyPlayer;
	UPROPERTY(EditAnywhere, Category = "Player Characters")
		int EnemyPlayerCount = 1;


	virtual void BeginPlay() override;
};
