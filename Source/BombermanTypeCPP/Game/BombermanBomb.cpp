// Fill out your copyright notice in the Description page of Project Settings.

#include "BombermanBomb.h"

#include "BombermanPlayer.h"

#include "../BombermanTypeCPP.h"
// Sets default values
ABombermanBomb::ABombermanBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attributes = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("Attributes"));


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABombermanBomb::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->OnAbilityEnded.AddUFunction(this, FName("DestroySelf"));
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABombermanBomb::Explode, FuseTime, false);

	if (AbilitySystemComponent)
	{
		if (HasAuthority() && ExplosionAbility)
		{
			BombAbilitySpec = FGameplayAbilitySpec(ExplosionAbility.GetDefaultObject(), 1, 0);
			AbilitySystemComponent->GiveAbility(BombAbilitySpec);
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}


}

// Called every frame
void ABombermanBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ABombermanBomb::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ABombermanBomb::InitializeAttributes()
{
	// If the ASC and DefaultAttributeEffect objects are valid
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		// Create context object for this gameplay effecct
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		// Create an outgoing effect spec using the effect to apply and the context
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			// Apply the effect using the derived spec
			// + Could be ApplyGameplayEffectToTarget() instead if we were shooting a target
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ABombermanBomb::GiveAbilities()
{
	// If the server has the authority to grant abilities and there is a valid ASC
	if (HasAuthority() && AbilitySystemComponent)
	{
		// Foreach ability in DefaultAbilities, grant the ability
		for (TSubclassOf<UGAS_GameplayAbility>& StartupAbility : DefaultAbilities)
		{
			// `1` below is the level of the ability, which could later be used to allow abilities to scale with player level
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ABombermanBomb::CallDestroy()
{
	Destroy();
}

void ABombermanBomb::DestroySelf()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABombermanBomb::CallDestroy, 0.0001f, false);
}

void ABombermanBomb::Explode()
{
	if (hasBeenDetonated)
		return;
	hasBeenDetonated = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Explode() at Bomb")));

	AbilitySystemComponent->TryActivateAbilityByClass(ExplosionAbility);
	//AbilitySystemComponent->TryActivateAbility(BombAbilitySpec.Handle);

}