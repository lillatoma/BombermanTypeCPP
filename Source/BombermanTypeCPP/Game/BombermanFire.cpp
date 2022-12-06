// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanFire.h"
#include "BombermanBomb.h"
#include "BombermanPlayer.h"
#include "BombermanFireDataHolder.h"

// Sets default values
ABombermanFire::ABombermanFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABombermanFire::BeginPlay()
{
	Super::BeginPlay();


	//Timed destroy
	//It can damage until explosiontime
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABombermanFire::CallDestroy, ExplosionTime, false);


	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABombermanFire::OverlapBegin);
}


void ABombermanFire::CallDestroy()
{
	//This function tells its firedata owner that it also can be destroyed
	//And destroys self after.
	if (FireData)
		FireData->InitiateDestroyCall();
	Destroy();
}

void ABombermanFire::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ABombermanBomb* Bomb = Cast<ABombermanBomb>(OtherActor);
	//If the fire hits a character, it gets added to the fire data's affected list
	//The affected list may only contain a character once, and they should get damaged.
	ABombermanPlayer* Player = Cast<ABombermanPlayer>(OtherActor);
	if (Player && FireData)
		FireData->AddPlayerToAffected(Player);
	//If the fire its a bomb, it makes it explode
	//Chain explosion
	if (Bomb && Bomb->HasBeenSetUp)
	{

		Bomb->Explode();
	}
	return;



}