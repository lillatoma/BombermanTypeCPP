// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanGameEndChecker.h"
#include "BombermanPlayer.h"
#include "BombermanTypeCPP\UI\EndingWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ABombermanGameEndChecker::ABombermanGameEndChecker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABombermanGameEndChecker::FindPlayerList()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABombermanPlayer::StaticClass(), FoundActors);

	int PNum = FoundActors.Num();
	for (int i = 0; i < PNum; i++)
	{
		ABombermanPlayer* Player = Cast<ABombermanPlayer>(FoundActors[i]);

		if (Player)
			PlayerList.Add(Player);
	}
}

// Called when the game starts or when spawned
void ABombermanGameEndChecker::BeginPlay()
{
	Super::BeginPlay();
	
	//Finding all players
	//Assuming that these players are spawned in less than 5 seconds
	//And that the game doesn't end that soon
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABombermanGameEndChecker::FindPlayerList, 5.f, false);

}

void ABombermanGameEndChecker::SpawnWidget(bool won)
{
	//This function spawns the widget if available
	//The game also becomes paused, so this in Tick() can only be called once
	
	if (GameWidgetClass)
	{
		GameWidget = Cast<UEndingWidget>(CreateWidget(GetWorld(), GameWidgetClass));

		if (GameWidget)
		{
			GameWidget->AddToViewport();
			GameWidget->SetWinloseText(won);

			APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerControllerRef->SetShowMouseCursor(true);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

// Called every frame
void ABombermanGameEndChecker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	int PNum = PlayerList.Num();
	for (int i = PNum - 1; i >= 0; i--)
	{
		bool bFound = false;

		float health = PlayerList[i]->GetAbilitySystemComponent()->GetGameplayAttributeValue(HealthAttribute, bFound);

		if (bFound && health <= 0)
		{
			//If it is the player who died, then the game ends with a lose
			if (PlayerList[i]->IsPlayerControlled())
			{
				SpawnWidget(false);
			}
			//If it is an enemy, we simply destroy it, and remove it from the list
			else
			{
				PlayerList[i]->Destroy();
				PlayerList.RemoveAt(i);

			}
		}
	}
	//If there is only one player left, and it is player controlled
	//Then the player won
	if (PlayerList.Num() == 1 && PlayerList[0]->IsPlayerControlled())
	{
		SpawnWidget(true);
	}
}

