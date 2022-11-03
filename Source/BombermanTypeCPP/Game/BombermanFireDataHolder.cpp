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
	PrimaryActorTick.bCanEverTick = true;


	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attributes = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("Attributes"));


}

// Called when the game starts or when spawned
void ABombermanFireDataHolder::BeginPlay()
{
	Super::BeginPlay();
	
	FindGrid();
	FindBombLength();
	CalculateGridPosition();
	SpawnFireFull();

	if (AbilitySystemComponent)
	{
		if (HasAuthority() && DamageAbility)
		{
			FGameplayAbilitySpec DamageAbilitySpec = FGameplayAbilitySpec(DamageAbility.GetDefaultObject(), 1, 0);
			AbilitySystemComponent->GiveAbility(DamageAbilitySpec);
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

}

// Called every frame
void ABombermanFireDataHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("GridActor Found")));
		if (Grid)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Grid Found")));
			MapGrid = Grid;
			return MapGrid;
		}
	}

	return nullptr;
}

void ABombermanFireDataHolder::FindBombLength()
{
	AActor* PlayerActor = GetOwner();

	if (PlayerActor)
	{
		ABombermanPlayer* Player = Cast<ABombermanPlayer>(PlayerActor);

		if (Player)
		{
			bool bFound = false;
			if (Player->GetAbilitySystemComponent())
			{
				ExplosionLength = Player->GetAbilitySystemComponent()
					->GetGameplayAttributeValue(BombLengthAttribute, bFound);
			}
		}
	}

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
	if (MapGrid->GetPointOnGrid(point)->Type != EMGPMapGridpointType::Solid)
	{
		FVector Location = MapGrid->ConvertGridToWorld(FIntPoint(point));
		Location.Z += 25.f;
		const FRotator Rotation = FRotator();
		AActor* FireObject = GetWorld()->SpawnActor<AActor>(FireActor, Location, Rotation);

		if (FireObject)
		{
			ABombermanFire* Fire = Cast<ABombermanFire>(FireObject);
			if (Fire)
				Fire->FireData = this;
		}

		if (MapGrid->GetPointOnGrid(point)->Type == EMGPMapGridpointType::Breakable)
		{
			MapGrid->GetPointOnGrid(point)->Type = EMGPMapGridpointType::Air;
			MapGrid->GetPointOnGrid(point)->Block->Destroy();
			MapGrid->GetPointOnGrid(point)->Block = nullptr;


			return false;
		}
		return true;
	}
	else return false;
}

void ABombermanFireDataHolder::SpawnFireFull()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SpawnFireFull()")));
	if (!MapGrid)
		FindGrid();
	if (!MapGrid)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ExpLength: %d"),ExplosionLength));

	int Left = ExplosionLength, Right = ExplosionLength, Up = ExplosionLength, Down = ExplosionLength;

	//LeftSide
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