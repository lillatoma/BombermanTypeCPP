// Fill out your copyright notice in the Description page of Project Settings.


#include "BombermanFireDataHolder.h"
#include "BombermanFire.h"
#include "BombermanPlayer.h"

#include "BombermanTypeCpp\Map\MapGrid.h"
#include "BombermanTypeCpp\Map\MapGridPoint.h"

#include "../BombermanTypeCPP.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABombermanFireDataHolder::ABombermanFireDataHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attributes = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("Attributes"));


}

// Called when the game starts or when spawned
void ABombermanFireDataHolder::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && DamageAbility)
		{
			FGameplayAbilitySpec DamageAbilitySpec = FGameplayAbilitySpec(DamageAbility.GetDefaultObject(), 1, 0);
			AbilitySystemComponent->GiveAbility(DamageAbilitySpec);
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	//Finding the grid, and position on it, and spawning fire accordingly
	//Without grid, nothing happens
	FindGrid();
	CalculateGridPosition();
	SpawnFireFull();



}


UAbilitySystemComponent* ABombermanFireDataHolder::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ABombermanFireDataHolder::InitializeAttributes()
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

void ABombermanFireDataHolder::GiveAbilities()
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

AMapGrid* ABombermanFireDataHolder::FindGrid()
{
	AActor* GridActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMapGrid::StaticClass());
	if (GridActor)
	{
		AMapGrid* Grid = Cast<AMapGrid>(GridActor);
		if (Grid)
		{
			MapGrid = Grid;
			return MapGrid;
		}
	}

	return nullptr;
}


void ABombermanFireDataHolder::CalculateGridPosition()
{
	FIntPoint GridPoint;

	if (!MapGrid)
		FindGrid();

	if(MapGrid)
		GridPoint = MapGrid->GetClosestGridPoint(GetActorLocation());
	CenterPoint = GridPoint;
}

bool ABombermanFireDataHolder::SpawnFire(FIntPoint point)
{
	
	UMapGridpoint* GP = MapGrid->GetPointOnGrid(point);
	//We should spawn a fire object, if it's not a solid wall
	if (GP && GP->Type != EMGPMapGridpointType::Solid)
	{
		FVector Location = MapGrid->ConvertGridToWorld(FIntPoint(point));
		const FRotator Rotation = FRotator();

		FTransform SpawnTransform(Rotation, Location, FVector(1, 1, 1));


		//Spawning a fire actor, but it has to get the firedata passed first
		ABombermanFire* Fire = Cast<ABombermanFire>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, FireActor, SpawnTransform));
		if (Fire)
		{
			Fire->FireData = this;
			UGameplayStatics::FinishSpawningActor(Fire, SpawnTransform);
		}
		//If the fire is spawned on a breakable, we destroy the breakable


		if (GP->Type == EMGPMapGridpointType::Breakable)
		{

			GP->Type = EMGPMapGridpointType::Air;
			if (GP->Block)
			{
				GP->Block->Destroy();
				GP->Block = nullptr;
			}
			MapGrid->RemoveBreakable();

			return false;
		}
		return true;

	}
	return false;
}

void ABombermanFireDataHolder::SpawnFireFull()
{

	if (!MapGrid)
		FindGrid();
	if (!MapGrid)
		return;
	int Left = ExplosionLength, Right = ExplosionLength, Up = ExplosionLength, Down = ExplosionLength;
	
	//Spawning on the centerpoint
	SpawnFire(CenterPoint);

	//LeftSide
	//We go as much towards the left, as much is possible
	//If the explosion reaches its max length, the condition won't be met anymore
	//If we hit a block, then the loop breaks
	for (int x = CenterPoint.X - 1; x > 0 && Left > 0; x--)
	{
		Left--;
		if (!SpawnFire(FIntPoint(x, CenterPoint.Y)))
			break;
	}
	//RightSide
	for (int x = CenterPoint.X + 1; x < MapGrid->GetSize().X - 1 && Right > 0; x++)
	{
		Right--;
		if (!SpawnFire(FIntPoint(x, CenterPoint.Y)))
			break;
	}

	//UpSide
	for (int y = CenterPoint.Y + 1; y > 0 && Up > 0; y++)
	{
		Up--;
		if (!SpawnFire(FIntPoint(CenterPoint.X, y)))
			break;
	}

	//DownSide
	for (int y = CenterPoint.Y - 1; y < MapGrid->GetSize().Y - 1 && Down > 0; y--)
	{
		Down--;
		if (!SpawnFire(FIntPoint(CenterPoint.X, y)))
			break;
	}
}

void ABombermanFireDataHolder::InitiateDestroyCall()
{
	//This function can only be called once
	//It tells the grid, that a bomb no longer exists at that point
	//Then initiates a timed destroy call
	if (hasBeenDestroyCalled)
		return;
	hasBeenDestroyCalled = true;


	if (MapGrid)
		MapGrid->DeleteBomb(CenterPoint);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ABombermanFireDataHolder::CallDestroy, DestroyWaitTime, false);
}

void ABombermanFireDataHolder::AddPlayerToAffected(ABombermanPlayer* Player)
{
	//It only adds the player on the list, if it's not on it
	if (AffectedPlayers.Num() == 0 || AffectedPlayers.Find(Player) == INDEX_NONE)
	{

		AffectedPlayers.Add(Player);
		///Calling the activation here, because I thought it wouldn't be good to monitor
		///the AffectedPlayers.Num() change in Tick()
		AbilitySystemComponent->TryActivateAbilityByClass(DamageAbility);
	}
}

void ABombermanFireDataHolder::SetAffectedPlayerCount(int count)
{
	AffectedCount = count;
}

void ABombermanFireDataHolder::CallDestroy()
{
	Destroy();
}