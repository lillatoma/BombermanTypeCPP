// Fill out your copyright notice in the Description page of Project Settings.


#include "MapBreakableBlock.h"

// Sets default values
AMapBreakableBlock::AMapBreakableBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AMapBreakableBlock::BeginPlay()
{
	Super::BeginPlay();

	DecideSpawnPowerup();

}

// Called every frame
void AMapBreakableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapBreakableBlock::DecideSpawnPowerup()
{
	float r = FMath::RandRange(0.f, 1.f);

	if (r < SpawnPercentage)
	{
		int idx = FMath::RandRange(0, PowerupList.Num() - 1);
		FVector Location = GetActorLocation();
		FRotator Rotation;
		AActor* PowerupObject = GetWorld()->SpawnActor<AActor>(PowerupList[idx], Location, Rotation);
	}
}