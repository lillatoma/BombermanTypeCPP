// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanFire.h"
#include "BombermanBomb.h"
#include "BombermanFireDataHolder.h"

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

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABombermanFire::OverlapBegin);
}

// Called every frame
void ABombermanFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombermanFire::CallDestroy()
{
	if (FireData)
		FireData->InitiateDestroyCall();
	Destroy();
}

void ABombermanFire::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABombermanBomb* Bomb = Cast<ABombermanBomb>(OtherActor);
	FString name = OtherActor->GetName();


	if (Bomb)
	{
		Bomb->Explode();
	}
}