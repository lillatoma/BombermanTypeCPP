// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanFire.h"

// Sets default values
ABombermanFire::ABombermanFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABombermanFire::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABombermanFire::CallDestroy, 1.f, false);
}

// Called every frame
void ABombermanFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombermanFire::CallDestroy()
{
	Destroy();
}