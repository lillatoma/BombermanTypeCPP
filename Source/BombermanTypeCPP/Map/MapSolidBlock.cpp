// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSolidBlock.h"

// Sets default values
AMapSolidBlock::AMapSolidBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AMapSolidBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapSolidBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

