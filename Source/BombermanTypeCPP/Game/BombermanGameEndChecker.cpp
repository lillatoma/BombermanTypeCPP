// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanGameEndChecker.h"
#include "BombermanPlayer.h"
#include "BombermanTypeCPP\UI\EndingWidget.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABombermanGameEndChecker::ABombermanGameEndChecker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABombermanGameEndChecker::BeginPlay()
{
	Super::BeginPlay();
	
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
			if (PlayerList[i]->IsPlayerControlled())
			{
				if (GameWidgetClass)
				{
					GameWidget = Cast<UEndingWidget>(CreateWidget(GetWorld(), GameWidgetClass));

					if (GameWidget)
					{
						GameWidget->AddToViewport();
						GameWidget->SetWinloseText(false);
					}
				}
			}
			else
			{
				PlayerList[i]->Destroy();
				PlayerList.RemoveAt(i);

			}
		}
	}

	if (PlayerList.Num() == 1 && PlayerList[0]->IsPlayerControlled())
	{
		if (GameWidgetClass)
		{
			GameWidget = Cast<UEndingWidget>(CreateWidget(GetWorld(), GameWidgetClass));

			if (GameWidget)
			{
				GameWidget->AddToViewport();
				GameWidget->SetWinloseText(true);
			}
		}
	}
}

