// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanPowerup.h"
#include "BombermanPlayer.h"


// Sets default values
ABombermanPowerup::ABombermanPowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;

	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"), true);
}

// Called when the game starts or when spawned
void ABombermanPowerup::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABombermanPowerup::OverlapBegin);
}

// Called every frame
void ABombermanPowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombermanPowerup::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABombermanPlayer* Player = Cast<ABombermanPlayer>(OtherActor);


	if (Player)
	{
		auto Context = Player->GetAbilitySystemComponent()->MakeEffectContext();
		Context.AddSourceObject(this);
		Player->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(
			PowerupEffect, Player->GetAbilitySystemComponent(), 1, Context);

		Destroy();
	}
}